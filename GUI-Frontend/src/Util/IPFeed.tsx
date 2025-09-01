import Hls from "hls.js"
import { useEffect, useRef, useState } from "react"

import "./IPFeed.css"

export default function CameraStream() {
    const videoRef = useRef<HTMLVideoElement>(null);
    const [currentCam, setCurrentCam] = useState(1);
    const [loading, setLoading] = useState(false);

    const URL = "http://localhost:4000/index.m3u8"

    async function waitForPlaylist(url: string, timeout = 15000) {
        const start = Date.now();
        while (Date.now() - start < timeout) {
            try {
                const res = await fetch(url, { method: "HEAD" });
                if (res.ok) return true; // Playlist exists
            } catch {}
                await new Promise(r => setTimeout(r, 500));
            }
        throw new Error("Playlist not ready");
    }

    const switchCamera = async (camId: number) => {
        if (camId == currentCam) return;
        setLoading(true);
        try {
            const res = await fetch(`http://localhost:4000/switch/${camId}`, {
                method: "POST",
            });
            const data = await res.json();
            if (!data.ok) throw new Error("Failed to switch stream");
            setCurrentCam(camId);

            await waitForPlaylist(`${URL}?t=${Date.now()}`);

            if (videoRef.current) {
                const hls = new Hls();
                hls.loadSource(`${URL}`);
                hls.attachMedia(videoRef.current);
            }
        } catch (err) {
            console.error("Error switching camera:", err)
        } finally {
            setLoading(false);
        }
    }

    useEffect(() => {
        if (videoRef.current) {
            if (Hls.isSupported()) {
                const hls = new Hls();
                hls.loadSource(URL);
                hls.attachMedia(videoRef.current);
                hls.on(Hls.Events.MANIFEST_PARSED, () => {
                    videoRef.current?.play();
                })
                return () => hls.destroy();
            } else if (videoRef.current.canPlayType("application/vnd.apple.mpegurl")) {
                videoRef.current.src = URL;
                videoRef.current.play();
            }
        }
    }, []);
    return (
        <div className="Player">
            <h1>Currently Showing Camera {currentCam}</h1>
            <video 
                ref={videoRef} 
                autoPlay 
                controls 
                style={{ width: "100%" }} 
            />
            <div className="Buttons">
                <button onClick={() => switchCamera(1)} disabled={loading}>
                    Camera 1
                </button>
                <button onClick={() => switchCamera(2)} disabled={loading}>
                    Camera 2
                </button>
            </div>
            {loading && <p>Switching camera...</p>}
        </div>
    );
}