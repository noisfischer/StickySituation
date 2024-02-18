import jwt from 'jsonwebtoken';
import { SECRET } from '@ss/constants';

export const payloadToToken = (payload: Profile): Promise<string | null> => {
  return new Promise(resolve => {
    jwt.sign(payload, SECRET, {}, (err, token) => {
      if (err) {
        console.error(err);
        return resolve(null);
      }
      resolve(token);
    });
  });
};

export const tokenToPayload = async function(token: string): Promise<Profile | null> {
  return new Promise(resolve => {
    jwt.verify(token, SECRET, { ignoreExpiration: true }, (err, decoded) => {
      if (err) return resolve(null);
      resolve(decoded as Profile);
    });
  });
};
