import { table } from './table';

export type Schema = {
  // date string to seed
  dates: Map<string, string>,
  // seed string to archives daily
  dailies: Map<string, {
    date: Date;
    seed: string;
    leaderboard: number[];
  }>,
};

export const dailyArchive = await table<Schema>('daily-archive', {
  dates: new Map(),
  dailies: new Map(),
});