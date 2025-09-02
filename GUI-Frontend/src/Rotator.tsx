import Header from "./Util/Header";

import "./Rotator.css"
import IPFeed from "./Util/IPFeed"
import { useState } from "react";


/**
 * This page is the page to control the rotator for the antennae, for the future integration, it needs to fetch the current azimuth and elevation of
 * the rotator/antenae
 * @returns the Rotator Page with IP Camera feed
 */
export default function Rotator() {
  const [elevation, setElevation] = useState(0);
  const [azimuth, setAzimuth] = useState(0);

  /**
   * The Button handle functions would be where to make requests to the backend
   * to increase or decrease the elevation or azimuth. Instead of using useState most likely
   * would be better to use useEffect to fetch the actual data from the system
   */
  function handleUpButton() {
    setElevation((elevation) => {return elevation + 5});
  }

  function handleDownButton() {
    setElevation((elevation) => {
      if (elevation < 5) {
        console.log("ELEVATION CAN'T BE NEGATIVE")
        return elevation
      }
      return elevation - 5;
    })
  }

  function handleClockwiseButton() {
    setAzimuth((azimuth) => {
      if (azimuth + 5 > 360) {
        return azimuth - 355
      }
      return azimuth + 5;
    });
  }

  function handleAntiClockwiseButton() {
    setAzimuth((azimuth) => {
      if (azimuth - 5 < 0) {
        return azimuth + 355;
      }
      return azimuth - 5;
    });
  }

  return (
  <div>
    <><Header /></>
    <div>
      <IPFeed/>
      <br />
    </div>
    <div className="rotButtons">
      <button className="rotButton" onClick={handleUpButton}>Up</button>
      <button className="rotButton" onClick={handleDownButton}>Down</button>
      <button className="rotButton" onClick={handleClockwiseButton}>Clockwise</button>
      <button className="rotButton" onClick={handleAntiClockwiseButton}>Anti-Clockwise</button>      
    </div>
    <div>
      <p className="data">{`Azimuth: ${azimuth} degrees | Elevation: ${elevation} degrees.`}</p>
    </div>
  </div>
  );
};


