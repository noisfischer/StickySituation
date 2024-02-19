import path from 'path';
import { Low } from 'lowdb';
import { DataFile } from 'lowdb/node';
import { VOLUME_PATH } from '@ss/constants';
import superjson from 'superjson';

export const table = async function<T>(name: string, defaultData: T) {
  const dbPath = path.resolve(VOLUME_PATH, `${name}.json`);
  
  const datafile = new DataFile<T>(dbPath, {
    parse: (str: string) => superjson.parse(str),
    stringify: (data: unknown) => superjson.stringify(data),
  });

  const db = new Low<T>(datafile, defaultData);

  await db.read();

  return db;
};

export type Table<T> = Low<T>;