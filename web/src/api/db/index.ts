import path from 'path';
import { JSONFilePreset } from 'lowdb/node';
import { VOLUME_PATH } from '@ss/constants';

const dbPath = path.resolve(VOLUME_PATH, 'db.json');

type Schema = {
  dailySeed: string;
  leaderboard: {
    id: string;
    score: number;
  }[];
  dailyLeaderboard: {
    id: string;
    score: number;
  }[];
};

export const db = await JSONFilePreset<Schema>(dbPath, {
  leaderboard: [],
  dailySeed: '',
  dailyLeaderboard: [],
});