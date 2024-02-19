import { table } from './table';

export type Schema = {
  leaderboard: number[];
};

export const leaderboard = await table<Schema>('leaderboard', {
  leaderboard: [],
});