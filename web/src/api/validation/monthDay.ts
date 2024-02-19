import { z } from 'zod';

export const monthDay = z.object({
  day: z.number().int().min(1).max(31),
  month: z.number().int().min(1).max(12),
});

export type MonthDay = z.infer<typeof monthDay>;