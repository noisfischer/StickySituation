import { table } from './table';
import { nanoid } from 'nanoid';
import { endOfDay } from 'date-fns';

export type Schema = {
  seed: string;
  date: Date;
  leaderboard: number[];
};

export const daily = await table<Schema>('daily', {
  seed: nanoid(),
  date: endOfDay(new Date()),
  leaderboard: [],
});