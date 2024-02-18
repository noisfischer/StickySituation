import { renderToString } from 'react-dom/server';
 
import App from './home';
 
export const render = () => {
  return renderToString(<App />);
};