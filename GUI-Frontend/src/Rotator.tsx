import Header from "./Util/Header";

import "./Rotator.css"
import IPFeed from "./Util/IPFeed"

export default function Rotator() {
  return (
  <div>
    <><Header /></>
    <div>
      <IPFeed/>
      <br />
    </div>
    <div className="rotButtons">
      <button className="rotButton">Clockwise</button>
      <button className="rotButton">Azimuth</button>
      <button className="rotButton">Anti-Clockwise</button>      
    </div>
  </div>
  );
};


