import { isEqual, endOfDay } from 'date-fns';
import { nanoid } from 'nanoid';
import { type Table } from '../db/table';
import { type Score } from '../validation/score';
import { type Profile } from '../validation/profile';
import { Leaderboard, serialize } from './leaderboard';
import { type Schema } from '../db/daily';
import { type Users } from './users';
import { type DailyArchive } from './daily-archive';

type JSONSchema = {
  date: string;
  seed: string;
  leaderboard: ReturnType<typeof serialize>;
};

export const Daily = function(db: Table<Schema>, users: Users, archive: DailyArchive, maxEntries: number) {
  let leaderboard = Leaderboard(db, users, maxEntries);
  let dailyUsers = new Set(leaderboard.getUserIds());

  return {
    now() {
      return endOfDay(new Date());
    },
    hasExpired() {
      // NOTE: this would be better as a CRON job
      // move to that later (this is a pretty simple
      // check to do in the meantime)
      return !isEqual(this.now(), db.data.date);
    },
    refresh() {
      // archive the data 
      archive.backup(db.data);

      // reset the data
      db.data.seed = nanoid();
      db.data.date = this.now();
      db.data.leaderboard = [];

      // refresh the leaderboard
      leaderboard = Leaderboard(db, users, maxEntries);
      dailyUsers = new Set(leaderboard.getUserIds());

      // write without waiting for the result
      db.write();
    },
    exists(profile: Profile) {
      return dailyUsers.has(profile.id);
    },
    add(profile: Profile, score: Score) {
      users.add(profile);

      // only allowed to do the daily once per day
      if (this.exists(profile)) return;

      // check if the score is valid for today's daily
      // if not, they missed the window for the daily
      const { seed } = db.data;
      if (score.seed !== seed) return;

      // add the user to the daily
      leaderboard.add(profile, score);
    },
    toConfig() {
      // right now, just the seed and the date
      return {
        seed: db.data.seed,
        date: db.data.date.toDateString(),
      };
    },
    toJSON(): JSONSchema {
      const { date, seed } = db.data;

      return {
        seed,
        date: date.toDateString(),
        leaderboard: leaderboard.toJSON(),
      };
    },
  };
};