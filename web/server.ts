import express from 'express';
import { middleware } from './src/server/middleware';
import { render } from './src/server/render';

const app = express();

middleware(app);

app.use('/', await render('home'));

app.listen(process.env.PORT, () => {
  console.log(`http://localhost:${process.env.PORT}`);
});