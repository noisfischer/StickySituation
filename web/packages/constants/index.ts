export const isDev = process.env.NODE_ENV !== 'production';
export const isProd = process.env.NODE_ENV === 'production';

// https://docs.railway.app/reference/volumes
// https://docs.railway.app/guides/volumes
export const VOLUME_NAME = process.env.RAILWAY_VOLUME_NAME || '.ss';
export const VOLUME_PATH = process.env.RAILWAY_VOLUME_MOUNT_PATH || '/.ss';

export const SECRET = process.env.SECRET || '';

export const base = process.env.BASE || '/';

export enum CHARACTER {
  JEREMY = 1,
  AMY,
  DENTIST,
};

export enum CHALLENGE {
  ONE = 1,
  TWO,
  THREE,
};

export const CHALLENGES = [
  CHALLENGE.ONE,
  CHALLENGE.TWO,
  CHALLENGE.THREE,
];