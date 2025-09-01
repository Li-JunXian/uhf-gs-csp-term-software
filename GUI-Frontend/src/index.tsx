import React from 'react';
import ReactDOM from 'react-dom/client';
import { createBrowserRouter, RouterProvider } from 'react-router';
import App from './App';
import Rotator from './Rotator';
import Header from './Util/Header';

import "./Global.css"

const rootEl = document.getElementById('root');
const router = createBrowserRouter([
  {
    path: '/',
    element: <App />,
    errorElement: <div><Header />404 Not Found</div>
  },
  {
    path: '/Rotator',
    element: <Rotator />
  }
]);

if (rootEl) {
  const root = ReactDOM.createRoot(rootEl);
  root.render(
    <React.StrictMode>
      <RouterProvider router={router} />
    </React.StrictMode>,
  );
}
