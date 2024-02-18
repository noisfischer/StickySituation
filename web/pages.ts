import fsp from 'fs/promises';

const files = await fsp.readdir('./src/pages');

export default files
  .filter(file => file.endsWith('.html'))
  .map(file => file.split('.').at(0));