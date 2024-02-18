import { isDev } from '@ss/constants';
import { type ViteDevServer, createServer } from 'vite';

export default isDev ? await createServer({
  appType: 'custom',
  server: { middlewareMode: true },
}) : null as unknown as ViteDevServer;