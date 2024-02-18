import { defineConfig } from 'vite';
import { fileURLToPath } from 'url';
import react from '@vitejs/plugin-react';

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [react()],

  build: {
    rollupOptions: {
      input: {
        home: fileURLToPath(new URL('./src/pages/home.html', import.meta.url)),
      },
    },
  },
  resolve: {
    // https://stackoverflow.com/questions/66043612/vue3-vite-project-alias-src-to-not-working
    alias: [
      { find: '~', replacement: fileURLToPath(new URL('./src', import.meta.url)) },
    ]
  },
});
