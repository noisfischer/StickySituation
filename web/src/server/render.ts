import path from 'path';
import vite from './vite';
import { hfs } from '@humanfs/node';
import { base, isDev, isProd, } from '@ss/constants';
import type { Request, Response } from 'express';
 
const cache = new Map<string, string>();

const viteRender = async (name: string) => {
  const src = path.resolve(process.cwd(), `src/pages/${name}.server.tsx`)
    .replace(/^(.*\/src)/, '/src');
  const { render } = await vite.ssrLoadModule(src);
  return render;
};

const liveRender = async (name: string) => {
  const src = path.resolve(process.cwd(), `dist/server/${name}.server.js`);
  const { render } = await import(src);
  debugger;
  return render;
};

export const render = function(name: string) {  
  const indexPath = isDev
    ? path.resolve(process.cwd(), `src/pages/${name}.html`)
    : path.resolve(process.cwd(), `dist/client/src/pages/${name}.html`);
  
  return async function(req: Request, res: Response) {
    if (cache.has(name)) return res.status(200).set({ 'Content-Type': 'text/html' }).end(cache.get(name));

    const url = req.originalUrl.replace(base, '');

    const template = isDev
      ? await vite.transformIndexHtml(url, await hfs.text(indexPath) || '')
      : await hfs.text(indexPath) || '';

    const render = isDev ? await viteRender(name) : await liveRender(name);
    const rendered = render();
    const html = template
      .replace(`<!--app-head-->`, rendered.head ?? '')
      .replace(`<!--app-html-->`, rendered.html ?? rendered ?? '');

    if (isProd) cache.set(name, html);

    res.status(200).set({ 'Content-Type': 'text/html' }).end(html);
  };
};