export const isDev = process.env.NODE_ENV !== 'production';
export const isProd = process.env.NODE_ENV === 'production';

// https://docs.railway.app/reference/volumes
// https://docs.railway.app/guides/volumes
export const VOLUME_NAME = process.env.RAILWAY_VOLUME_NAME || '.volume';
export const VOLUME_PATH = process.env.RAILWAY_VOLUME_MOUNT_PATH || '/.volume';

export const SECRET = process.env.SECRET || '376ea5af478b9b6b90119f454548b0795f47965ddad86291922212817f42992ab579fdc90df9b52ca6f517694598e615c12afad46eaad4d2285326aa589a2ad84540c61960b5aad068e8f50cad5ae7c85826a0ef70c270a52fb33af2a0b9b4c1cf71b5880354191e6c6da08c90e4aae2438a93cf3f8642427b58dc655fdfab306bdce2967ccd0ff5b63273ef9a2e4ae5576b827d3f41cc96a08d18375cb63cb3de9e0ff73e68dde8044d7a9dd3bc80821a859ff87eab910884ede3334a31b9945fc49b2ec4ee6440a3bed50c9df122e870a45aafeb172bbcd0655760cebdb938b76608981cfd3404f065116502cef28f5207cfaa50137de1d33550ae2c250249';
export const TOKEN_DURATION = 60; // in seconds

// how many records should we keep for each leaderboard? can increase if
// playerbase increases 
export const MAX_ENTRIES = 1000;

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