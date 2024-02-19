import { z } from 'zod';

export const seed = z.string().max(21); // nanoid
export const points = z.number().min(0).max(Number.MAX_SAFE_INTEGER).int();

export const score = z.object({
  seed,
  score: points,
});

export type Score = z.infer<typeof score>;