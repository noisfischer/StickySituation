import { middleware as securityMiddleware } from './security';
import { isEqual, endOfDay, setDay, setMonth } from 'date-fns';

import { monthDay as monthDayValidation } from './validation/monthDay';
import {
  score as scoreValidation,
  points as pointsValidation,
} from './validation/score';

import { daily as dailyDb } from './db/daily';
import { users as usersDb } from './db/users';
import { leaderboard as leaderboardDb } from './db/leaderboard';
import { dailyArchive as dailyArchiveDb } from './db/daily-archive';

import { Daily } from './domain/daily';
import { Users } from './domain/users';
import { Leaderboard } from './domain/leaderboard';
import { DailyArchive } from './domain/daily-archive';

import bodyParser from 'body-parser';

import {
  MAX_ENTRIES,
} from '@ss/constants';
import {
  Router,
  type Request,
  type Response,
} from 'express';


// setup domain logic with the databases

const users = Users(usersDb);
const leaderboard = Leaderboard(leaderboardDb, users, MAX_ENTRIES);
const dailyArchive = DailyArchive(dailyArchiveDb, users);
const daily = Daily(dailyDb, users, dailyArchive, MAX_ENTRIES);


// setup the parser for JSON data coming into the server (used below)
// the most we SHOULD get at this point is a formatted score
// object:
/*{
  "seed": "ZZZZZZZZZZZZZZZZZZZZZ",
  "score": 9007199254740991
}*/
// which is 68 bytes, so we'll limit it to 1kb just to be safe
const json = bodyParser.json({
  limit: '1kb',
});


// setup the routes, making sure they use the security middleware
// for talking to the server

const leaderboardRoute = Router();
const dailyLeaderboardRoute = Router();

leaderboardRoute.use(securityMiddleware);
dailyLeaderboardRoute.use(securityMiddleware);


// leaderboard logic

leaderboardRoute.post('/', json, (req: Request, res: Response) => {
  const { success } = pointsValidation.safeParse(req.body?.score);
  if (!success) return res.status(400).json({ error: 'Invalid score' });
  
  leaderboard.add(req.user, req.body);

  return res.json(leaderboard.toJSON());
});

leaderboardRoute.get('/', (req: Request, res: Response) => {
  return res.json(leaderboard.toJSON());
});


// daily archive logic

dailyLeaderboardRoute.get('/archive/:month/:day', (req: Request, res: Response) => {
  if (daily.hasExpired()) daily.refresh();

  const params = {
    day: +req.params.day,
    month: +req.params.month,
  };
  const { success } = monthDayValidation.safeParse(params);
  if (!success) return res.status(400).json({ error: 'Invalid date' });
  
  const date = endOfDay(setDay(setMonth(new Date(), params.month), params.day));
  
  // the long way of getting today's daily
  if (isEqual(date, daily.now())) return res.json(daily.toJSON());

  // retrive from archive
  const data = dailyArchive.toJSON(date);
  if (!data) return res.status(404).json({ error: 'No daily found' });

  res.json(data);
});


// daily leaderboard logic

dailyLeaderboardRoute.get('/config', (req: Request, res: Response) => {
  if (daily.hasExpired()) daily.refresh();
  
  return res.json(daily.toConfig());
});

dailyLeaderboardRoute.post('/', json, (req: Request, res: Response) => {
  if (daily.hasExpired()) daily.refresh();
  
  const { success } = scoreValidation.safeParse(req.body);
  if (!success) return res.status(400).json({ error: 'Invalid score' });

  daily.add(req.user, req.body);

  return res.json(daily.toJSON());
});

dailyLeaderboardRoute.get('/', (req: Request, res: Response) => {
  if (daily.hasExpired()) daily.refresh();

  return res.json(daily.toJSON());
});


// expose the routes

export {
  leaderboardRoute as leaderboard,
  dailyLeaderboardRoute as dailyLeaderboard,
};