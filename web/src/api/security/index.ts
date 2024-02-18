import jwt from 'jsonwebtoken';
import { type Request } from 'express';
import {
  SECRET,
  TOKEN_DURATION,
  
  type Profile,
} from '@ss/constants';

export const parseAuthorizationHeader = (req: Request) => {
  const { authorization = '' } = req.headers;
  return authorization.split(' ').at(1) ?? '';
};

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