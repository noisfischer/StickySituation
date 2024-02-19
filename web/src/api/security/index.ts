import jwt from 'jsonwebtoken';
import {
  type Request,
  type Response,
  type NextFunction,
} from 'express';
import {
  SECRET,
  TOKEN_DURATION,
} from '@ss/constants';
import {
  profile,
  type Profile,
} from '../validation/profile';

declare global {
  namespace Express {
    interface Request {
      user: Profile;
    }
  }
}

export const payloadToToken = (payload: Profile): Promise<string | undefined> => {
  return new Promise(resolve => {
    jwt.sign(payload, SECRET, { expiresIn: TOKEN_DURATION }, (err, token) => {
      if (err) {
        console.error(err);
        return resolve(undefined);
      }
      resolve(token);
    });
  });
};

export const tokenToPayload = async function(token: string): Promise<Profile | undefined> {
  return new Promise(resolve => {
    jwt.verify(token, SECRET, (err, decoded) => {
      if (err) return resolve(undefined);
      resolve(decoded as Profile);
    });
  });
};

const parseAuthorizationHeader = (req: Request) => {
  const { authorization = '' } = req.headers;
  return authorization.split(' ').at(1) ?? '';
};

export const middleware = async function(req: Request, res: Response, next: NextFunction) {
  const token = parseAuthorizationHeader(req);
  if (!token) return res.status(401).json({ error: 'Unauthorized' });

  const payload = await tokenToPayload(token);
  if (!payload) return res.status(401).json({ error: 'Invalid token' });

  const { success } = profile.safeParse(payload);
  if (!success) return res.status(401).json({ error: 'Invalid token' });

  req.user = payload;

  next();
};