import express from 'express';
import { middleware } from './src/server/middleware';
import { render } from './src/server/render';
import { leaderboard, dailyLeaderboard } from './src/api';

const app = express();

// api routes
app.use('/api/v1/leaderboard/', leaderboard);
app.use('/api/v1/daily/leaderboard/', dailyLeaderboard);

// setup middleware for the web server
middleware(app);

// web routes
app.use('/', await render('home'));

app.listen(process.env.PORT, () => {
  console.log(`http://localhost:${process.env.PORT}`);
});