import { useState, useMemo, type ChangeEvent, type FormEvent, useEffect } from "react";
import earthImg from "../assets/earth-day.jpg"
import earthTop from "../assets/earth-topology.png"
import nightSky from "../assets/night-sky.png"
import "./App.css";
import Header from "./Util/Header";
import PopupMenu from "./Util/Popup";
import Globe from "react-globe.gl";
import { generateOrbit } from "./Util/Predictor"

/**
 * This type describes the location of a station and its name
 */
type labels = {
  name: string;
  lat: number;
  lng: number;
};

/**
 * This type describes a single position of a satellite
 */
type SatPos = {
  lat : number, 
  lng : number, 
  altitude : number
};

/**
 * This type describes the entire orbit of a satellite with its name, when its orbit was last updated and its entire path
 */
type SatStruct = {
  name : string,
  lastUpdate : Date
  path : number[][]
}

/**
 * This type describes the current position of a satellite
 */
type PartStruct = {
  name : string,
  point : number[]
}

interface FormProp {
  inputLat: number;
  inputLong: number;
  inputName: string;
  isAdding: boolean[];
  handleSubmit: (e: FormEvent<HTMLFormElement>) => void;
  handleNames: (e: ChangeEvent<HTMLInputElement>) => void;
  handleLat: (e: ChangeEvent<HTMLInputElement>) => void;
  handleLong: (e: ChangeEvent<HTMLInputElement>) => void;
}

/**
 * Dummy satellite data gotten from a website. In the future maybe use a JSON to store the TLE data when it is updated so
 * that when the data is updated it will re render the program.
 */
const satellites = [{
  name : "SCOOB-II",
  lastUpdate : new Date(Date.now()),
  TLE1 : "1 63211U 25052B   25237.20035185  .00010988  00000-0  47621-3 0  9999",
  TLE2 : "2 63211  97.4291 130.4139 0006211  71.3692 288.8218 15.22533988 24904"
}, {
  name : "NUSHSat1",
  lastUpdate : new Date(Date.now()),
  TLE1 : "1 57484U 23109D   24006.48621913  .00017746  00000-0  90245-3 0  9997",
  TLE2 : "2 57484   4.9510 145.7547 0005430 219.0242 140.9473 15.13763583 24325"
}]

const AddForm = ({
  inputLat,
  inputLong,
  inputName,
  isAdding,
  handleSubmit,
  handleNames,
  handleLat,
  handleLong,
}: FormProp) => {
  return (
    <form id="inputForm" onSubmit={handleSubmit}>
      <label>{isAdding[1] ? "Satellite Name:" : "Station Name:"}</label>
      <input
        type="text"
        autoFocus
        name="name"
        value={inputName}
        onChange={handleNames}
      />
      <br />
      <label>Latitude:</label>
      <input
        type="number"
        name="latitude"
        value={inputLat}
        onChange={handleLat}
      />
      <br />
      <label>Longtitude:</label>
      <input
        type="number"
        name="longtitude"
        value={inputLong}
        onChange={handleLong}
      />
      <br />
      <input type="submit" />
    </form>
  );
};

const App = () => {
  const [isAdding, setIsAdding] = useState([false, false, false]);
  const [inputName, setInputName] = useState<string>("");
  const [inputLat, setInputLat] = useState(0);
  const [inputLong, setInputLong] = useState(0);
  const [hoveredPoint, setHoveredPoint] = useState<object | null>(null);
  const [mousePos, setMousePos] = useState({x : 0, y : 0});
  const [isPopped, setIsPopped] = useState(false);
  const [clickedPoint, setClickedPoint] = useState("");
  const [elapsedTime, setElapsedTime] = useState(0);
  const [isMax, setIsMax] = useState(false);
  
  const [lData, setLData] = useState<labels[]>([
    {
      name: "STAR E7",
      lat: 1.29,
      lng: 103.77,
    },
  ]);

  const handleNames = (e: ChangeEvent<HTMLInputElement>) => {
    setInputName(e.target.value);
  };
  const handleLat = (e: ChangeEvent<HTMLInputElement>) => {
    setInputLat(parseFloat(e.target.value));
  };
  const handleLong = (e: ChangeEvent<HTMLInputElement>) => {
    setInputLong(parseFloat(e.target.value));
  };

  const handleSubmit = (e: FormEvent<HTMLFormElement>) => {
    e.preventDefault();
    let tempLabel: labels = { name: inputName, lat: inputLat, lng: inputLong };
    if (isAdding[2]) {
      setLData([...lData, tempLabel]);
    }
    console.log("satData", satData, Array.isArray(satData));
    setIsAdding([false, false, false]);
    setInputName("");
    setInputLat(0);
    setInputLong(0);
  };

  useEffect(() => { //counter to update elapsedTime by 1 every minute
    const minuteCounter = setInterval(() => {
      setElapsedTime((elapsedTime) => {
        if (elapsedTime >= 96) {
          setIsMax(true);
          return 0;
        } else if (isMax && elapsedTime < 96) setIsMax(false);
        return elapsedTime + 1
      });
    }, 60000);
    return () => clearInterval(minuteCounter)
  }, [])
      
  const satData = useMemo( //Variable that populates/repopulates itself with the coordinates of the orbit of a satellite
    () => {
      console.log("generated");
      const data = satellites.map((satellite) => { 
        return { 
          name : satellite.name, 
          lastUpdate : satellite.lastUpdate, 
          path : generateOrbit(satellite.TLE1, satellite.TLE2) 
        }
      })
      return data;
    }, [satellites, isMax]
  );
  
  /**
   * Uses the indexes of the orbit array to track the live location of the satellite based on the TLE generated orbit
   * uses the timer above which updates elapsedTime to track which index to be on
  */
  const particlesData = useMemo( 
    () => {
      var data : PartStruct[] = satData.map((sat) => {
        var temp = sat.path[elapsedTime]
          return {
            name : sat.name,
            point : temp
          }
      })
      return data;
  }, [satellites, elapsedTime])
  return (
    <div className="content">
      <Header />
      {isAdding[0] ? (
        <div id="selector">
          <button
            id="closeButton"
            onClick={() => setIsAdding([false, isAdding[1], isAdding[1]])}
          >
            <svg
              xmlns="http://www.w3.org/2000/svg"
              x="0px"
              y="0px"
              width="28"
              height="28"
              viewBox="0 0 48 48"
            >
              <path
                fill="#F44336"
                d="M21.5 4.5H26.501V43.5H21.5z"
                transform="rotate(45.001 24 24)"
              ></path>
              <path
                fill="#F44336"
                d="M21.5 4.5H26.5V43.501H21.5z"
                transform="rotate(135.008 24 24)"
              ></path>
            </svg>
          </button>
          <button
            id="addSatellite"
            onClick={() => setIsAdding([false, true, false])}
          >
            Satellite
          </button>
          <button
            id="addStation"
            onClick={() => setIsAdding([false, false, true])}
          >
            Station
          </button>
        </div>
      ) : isAdding[1] || isAdding[2] ? (
        <AddForm
          inputLat={inputLat}
          inputLong={inputLong}
          inputName={inputName}
          isAdding={isAdding}
          handleSubmit={handleSubmit}
          handleLat={handleLat}
          handleLong={handleLong}
          handleNames={handleNames}
        />
      ) : (
        <button
          id="addbutton"
          name="Add"
          onClick={() => setIsAdding([true, isAdding[1], isAdding[2]])}
        >
          +
        </button>
      )}
      <div className="Globe" 
        onClick={(e) => {
          setMousePos({x : e.clientX, y: e.clientY})
          if (isPopped) {
            setIsPopped(false);
          }
        }}
        >
        <PopupMenu 
          hoveredPoint={hoveredPoint as SatPos | null} 
          mousePos={mousePos} 
          isPopped={isPopped}
          name={clickedPoint}
          />
        <Globe
          backgroundColor="#030a18"
          backgroundImageUrl={nightSky}
          bumpImageUrl={earthTop}
          globeImageUrl={earthImg}
          labelsData={lData}
          labelLat={(d) => (d as labels).lat}
          labelLng={(d) => (d as labels).lng}
          labelText={(d) => (d as labels).name}
          labelSize={1}
          labelDotRadius={0.1}
          pathsData={satData}
          pathLabel="name"
          pathPoints="path"
          pathColor={() => ["rgba(27, 255, 46, 1)", "rgba(254, 230, 9, 1)"]}
          onPathClick={(path, event, coords) => {
            setHoveredPoint(coords)
            setClickedPoint((path as SatStruct).name);
            setIsPopped(true);
          }}
          
          pathDashLength={0.01}
          pathDashGap={0.004}
          pathDashAnimateTime={100000}
          pathPointAlt={(pnt) => pnt[2]}
          pathTransitionDuration={4000}
          particlesData={particlesData}
          particlesList={particlesData}
          particleLat={(d) => (d as PartStruct).point[0]}
          particleLng={(d) => (d as PartStruct).point[1]}
          particleAltitude={(d) => (d as PartStruct).point[2]}
          particlesSize={7}
        />
      </div>
    </div>
  );
};

export default App;
