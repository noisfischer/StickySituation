import Alea from 'alea';
import { nanoid } from 'nanoid';

export interface RandomInstance {
  (): number;
  seed: string;
  random: () => number;
  base: (_num: number) => number;
  between: (_lower: number, _upper: number) => number;
  sample: <T>(_arr: T[]) => T | undefined;
  chance: (_perc?: number) => boolean;
  shuffle: <T>(_array: T[]) => T[];
  import: (_state: number[]) => RandomInstance;
  export: () => number[];
  fork: (_seed?: string) => RandomInstance;
};

const proto = Object.assign(Object.create(Function.prototype), {
  // is overriden by Alea
  seed: '',
  random() { return 0; },

  base(num: number): number {
    return -this.random() * num + this.random() * num;
  },

  between(lower: number, upper: number): number {
    return lower + Math.floor(this.random() * (upper - lower + 1));
  },

  // get an item from an array
  sample<T>(arr: T[]): T | undefined {
    if (!arr.length) return;
    if (arr.length === 1) return arr[0];
    return arr[this.between(0, arr.length - 1)];
  },

  // chance
  chance(perc: number = 0.5): boolean {
    return this.random() < perc;
  },

  // https://bost.ocks.org/mike/shuffle/
  shuffle<T>(array: T[]): T[] {
    let length = array.length;
    
    // while there remain elements to shuffle…
    while (length) {
      // pick a remaining element…
      const index = Math.floor(this.random() * length--);

      // And swap it with the current element.
      const current = array[length];
      array[length] = array[index];
      array[index] = current;
    }

    return array;
  },

  mul(x: number): number {
    return this.random() * x;
  },
  
  div(x: number): number {
    return this.random() / x;
  },

  sign(): number {
    return this.chance() ? 1 : -1;
  },

  import(state: number[]) {
    // @ts-ignore
    this.random.importState(state);
    return this;
  },

  export(): number[] {
    // @ts-ignore
    return this.random.exportState();
  },

  fork(seed = '-') {
    // eslint-disable-next-line no-use-before-define
    return Random(`${this.seed}${seed}`);
  },
});

export const Random = function(seed: string = nanoid()): RandomInstance {
  /* @ts-ignore */
  const random = new Alea(seed);
  random.seed = seed;
  random.random = random;
  random.__proto__ = proto;
  return random;
};

export const random = Random();

export const id = (precision = 5) => nanoid(precision);