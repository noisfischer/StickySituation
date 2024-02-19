import { type Table } from '../db/table';
import { type Users } from './users';
import { type Schema as ArchiveSchema } from '../db/daily-archive';
import { type Schema as DailySchema } from '../db/daily';
import { serialize } from './leaderboard';

type JSONSchema = {
  date: string;
  seed: string;
  leaderboard: ReturnType<typeof serialize>;
};

export const DailyArchive = function(db: Table<ArchiveSchema>, users: Users) {
  return {
    backup(daily: DailySchema) {
      const { dates, dailies } = db.data;

      dates.set(daily.date.toDateString(), daily.seed);
      dailies.set(daily.seed, daily);
      
      // write without waiting for the result
      db.write();
    },
    toJSON(date: Date): JSONSchema | null {
      const { dates, dailies } = db.data;
      
      const dateString = date.toDateString();
      const seed = dates.get(dateString);
      if (!seed) return null;
      
      const daily = dailies.get(seed);
      if (!daily) return null;

      return {
        seed,
        date: dateString,
        leaderboard: serialize(users, daily.leaderboard),
      };
    },
  };
};

export type DailyArchive = ReturnType<typeof DailyArchive>;