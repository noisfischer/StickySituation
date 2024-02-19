import { z } from 'zod';

export const id = z.number().int();
export const name = z.string().min(1).max(256);

export const profile = z.object({
  id,
  name,
});

export type Profile = z.infer<typeof profile>;