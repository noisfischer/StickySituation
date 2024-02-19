import { type Score } from '../validation/score';
import { type Profile } from '../validation/profile';
import { type Table } from '../db/table';
import { type Users } from './users';
import { type Schema as LeaderboardSchema } from '../db/leaderboard';
import { type Schema as DailySchema } from '../db/daily';

type JSONSchema = {
  name: string;
  score: number;
}[];

export const serialize = (users: Users, leaderboard: number[]): JSONSchema => {
  const results = new Array(leaderboard.length / 2);
  for (let i = 0; i < leaderboard.length; i += 2) {
    const name = users.get(leaderboard[i]);
    const score = leaderboard[i + 1];
    results[i / 2] = {
      name,
      score,
    };
  }
  return results;
};

export const Leaderboard = function(db: Table<LeaderboardSchema> | Table<DailySchema>, users: Users, maxEntries: number) {
  const max = maxEntries * 2;

  // we've optimized this for write performance (since that's
  // what leaderboards are least performant at), and we're
  // assuming that the leaderboard is read much more often
  // than it's written. so we're dirt checking the leaderboard
  // and caching the JSON representation
  let isDirty = true;
  let cache: JSONSchema = [];

  return {
    add(profile: Profile, score: Score) {
      const { leaderboard } = db.data;

      users.add(profile);

      let inserted = false;
      // walk backwards through the leaderboard, as the occurance of
      // a new score is likely to be closer to the end of the list
      // for a larger volume of data
      for (let i = leaderboard.length - 2; i >= 0; i -= 2) {
        if (score.score > leaderboard[i + 1]) {
          leaderboard.splice(i + 2, 0, profile.id, score.score);
          inserted = true;
          break;
        }
      }
      if (!inserted && leaderboard.length < max) {
        inserted = true;
        leaderboard.push(profile.id, score.score);
      }

      if (!inserted) return;

      isDirty = true;

      // truncate the leaderboard to the max length
      if (leaderboard.length > max) leaderboard.length = max;

      // write without waiting for the result
      db.write();
    },
    getUserIds(): number[] {
      const { leaderboard } = db.data;
      
      const userIds = new Array(leaderboard.length / 2);
      for (let i = 0; i < leaderboard.length; i += 2) {
        userIds[i / 2] = leaderboard[i];
      }

      return userIds;
    },
    toJSON(): JSONSchema {
      if (!isDirty) return cache;
      isDirty = false;

      const { leaderboard } = db.data;
      cache = serialize(users, leaderboard);
      
      return cache;
    },
  };
};

export type Leaderboard = ReturnType<typeof Leaderboard>;