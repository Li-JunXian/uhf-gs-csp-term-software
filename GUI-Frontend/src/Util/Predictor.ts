import * as satellite from "satellite.js";

/**
 * This function generates the orbit from the 2 lines of the TLE of a satellite
 * for the next 95 minutes which is about 1 orbital period
 * @param TLE1 line 1 of the TLE
 * @param TLE2 line 2 of the TLE
 * @returns the entire orbit as an array of triplets with [lat, lng, alt/1000] with 95 indexes
 */
export function generateOrbit(TLE1 : string, TLE2 : string) {
    const satrec = satellite.twoline2satrec(TLE1, TLE2);
    var orbitData : number[][] = [];
    // Propagate over ~1 orbit (≈ 95 minutes)
    for (let m = 0; m <= 95; m += 1) {
        const time = new Date(Date.now() + m * 60000); // now + m minutes
        const pv = satellite.propagate(satrec, time);
        if (pv != null && pv.position) {
            const gmst = satellite.gstime(time);
            const gd = satellite.eciToGeodetic(pv.position, gmst);
            let lat = Number(satellite.degreesLat(gd.latitude).toFixed(2));
            let lng = Number(satellite.degreesLong(gd.longitude).toFixed(2));
            let alt = Number(gd.height.toFixed(2));
            let newCoord = [lat, lng, alt/1000] 
            // console.log(
            //     `T+${m} min → Lat: ${lat}°, ` +
            //     `Lon: ${lng}°, ` +
            //     `Alt: ${alt} km`
            // );
            orbitData.push(newCoord)
        }
    }
    return orbitData;
}
// Your TLE
