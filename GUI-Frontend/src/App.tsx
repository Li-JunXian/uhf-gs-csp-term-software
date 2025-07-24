import { useState, useMemo, type ChangeEvent, type FormEvent } from "react";
import "./App.css";
import Header from "./Util/Header";
import Globe from "react-globe.gl";

type labels = {
  //this type is the data for stations!
  name: string;
  lat: number;
  lng: number;
};

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
      console.log(inputName);
      console.log(inputLat);
      console.log(inputLong);
      setLData([...lData, tempLabel]);
      console.log(lData);
    }
    setIsAdding([false, false, false]);
    setInputName("");
    setInputLat(0);
    setInputLong(0);
  };

  const satData = useMemo(
    () =>
      [...Array(1).keys()].map(() => {
        let lat = 1.29;
        let lng = 103.77;
        let alt = 0.5;

        // let lat2 = 20;
        // let lng2 = 20;
        // let alt2 = 1;

        return [
          [lat, lng, alt],
          ...[...Array(Math.round(359)).keys()].map(() => {
            lat = lat;
            lng = lng >= 180 ? 0 : lng + 1;
            alt = Math.max(0, alt);

            return [lat, lng, alt];
          }),
        ];
      }),
    []
  );
  console.log(satData);
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
      <Globe
        backgroundColor="#030a18"
        backgroundImageUrl="//cdn.jsdelivr.net/npm/three-globe/example/img/night-sky.png"
        bumpImageUrl={
          "//cdn.jsdelivr.net/npm/three-globe/example/img/earth-topology.png"
        }
        globeImageUrl="//cdn.jsdelivr.net/npm/three-globe/example/img/earth-day.jpg"
        labelsData={lData}
        labelLat={(d) => (d as labels).lat}
        labelLng={(d) => (d as labels).lng}
        labelText={(d) => (d as labels).name}
        labelSize={1}
        labelDotRadius={0.1}
        pathsData={satData}
        pathColor={() => ["rgba(0,0,255,0.6)", "rgba(255,0,0,0.6)"]}
        pathDashLength={0.01}
        pathDashGap={0.004}
        pathDashAnimateTime={100000}
        pathPointAlt={(pnt) => pnt[2]}
        pathTransitionDuration={4000}
      />
    </div>
  );
};

export default App;
