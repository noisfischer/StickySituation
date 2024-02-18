import React from 'react';
import App from './home';
import { hydrateRoot } from 'react-dom/client';
 
hydrateRoot(document.getElementById('root')!, 
  <React.StrictMode>
    <App />
  </React.StrictMode>,
);