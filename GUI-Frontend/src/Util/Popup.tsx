import "./Popup.css"

type SatPos = { lat : number, lng : number, altitude : number};
type coord = {x : number, y : number}

interface PopupInterface {
    hoveredPoint : SatPos | null,
    mousePos : coord,
    isPopped : boolean,
    name : string
}

export default function PopupMenu( { hoveredPoint : hoveredPoint, mousePos : mousePos, isPopped : isPopped, name : name}: PopupInterface) {
    return (
    (hoveredPoint && isPopped) ? (<div 
        className="popup"
        style={{
            opacity: 0.5,
            position: "absolute",
            top: mousePos.y + 12, 
            left: mousePos.x + 12
        }}
    >
        <header className="name">{`${name}`}</header>
        {`Lat: ${hoveredPoint.lat.toFixed(2)}, Lng: ${hoveredPoint.lng.toFixed(2)}, Alt: ${(hoveredPoint.altitude * 1000).toFixed(2)}`}
    </div>) : null
    )
}