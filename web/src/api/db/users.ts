import { table } from './table';

export type Schema = {
  users: Map<number, string>;
};

export const users = await table<Schema>('users', {
  users: new Map(),
});