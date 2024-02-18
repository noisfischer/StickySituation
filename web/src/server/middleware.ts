import { type Express } from 'express';
import vite from './vite';
import sirv from 'sirv';
import compression from 'compression';
import { base, isDev } from '@ss/constants';

export const middleware = (app: Express) => {
  if (isDev) {
    app.use(vite.middlewares);
    return;
  }

  app.use(compression());
  app.use(base, sirv('./dist/client', { extensions: [] }));
};