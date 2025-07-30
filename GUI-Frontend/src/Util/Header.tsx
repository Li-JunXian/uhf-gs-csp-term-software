import "./Header.css";

export default function Header() {
  return (
    <nav className="nav">
      <a href="/" className="site-title">
        Globe
      </a>
      <ul>
        <li className="active">
          <a href="/Rotator">Rotator</a>
        </li>
        <li>
          <a href="Settings">Settings</a>
        </li>
      </ul>
    </nav>
  );
}
