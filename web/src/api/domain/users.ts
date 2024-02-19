import { type Profile } from '../validation/profile';
import { type Table } from '../db/table';

type UserSchema = {
  users: Map<number, string>;
};

export const Users = function(usersDb: Table<UserSchema>) {
  return {
    get(id: number): string {
      return usersDb.data.users.get(id) ?? '';
    },
    add(profile: Profile) {
      const { users } = usersDb.data;
      if (users.has(profile.id)) return;
      
      users.set(profile.id, profile.name);
      usersDb.write();
    },
  };
};

export type Users = ReturnType<typeof Users>;