import { Link } from "react-router";

import "./Header.css";

export default function Header() {
  return (
    <nav className="nav">
      <a href="/" className="site-title">
        Globe
      </a>
      <ul>
        <li className="active">
          <Link to="/Rotator">Rotator</Link>
        </li>
        <li>
          <Link to="/Settings">Settings</Link>
        </li>
      </ul>
    </nav>
  );
}
