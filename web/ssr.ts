import { $ } from 'bun';
import pages from './pages';

for (const name of pages) {
  await $`vite build --ssr src/pages/${name}.server.tsx --outDir dist/server`;
}