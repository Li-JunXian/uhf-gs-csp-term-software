const express = require("express");
const { spawn } = require("child_process");
const path = require("path");
const cors = require("cors");
const fs = require("fs")
const { start } = require("repl");

const port = 4000;

const rtspUrls = {
  1: "rtsp://star_admin1:fyPe40801_2022@172.20.46.12:554/Streaming/Channels/101/",
  2: "rtsp://star_admin2:fyPe40801_2022@172.20.46.13:554/Streaming/Channels/101/",
};

const hlsDir = path.join(__dirname, "public", "hls", "active");
const hlsFile = path.join(hlsDir, 'index.m3u8');

if (!fs.existsSync(hlsDir)) fs.mkdirSync(hlsDir, {recursive: true});

let ffmpegProc = null;

function cleanHlsOutput() {
  for (const f of fs.readdirSync(hlsDir)) {
    if (f.endsWith(".m3u8") || f.endsWith(".ts") || f.endsWith(".tmp")) {
     try { fs.unlinkSync(path.join(hlsDir, f)); } catch {}
    }
  }
}

const startFFmpeg = (rtspUrl) => {
  const ffmpeg = spawn("ffmpeg", [
    "-rtsp_transport", "tcp",
    "-i", rtspUrl,
    "-c:v", "libx264",
    "-preset", "veryfast",
    "-tune", "zerolatency",
    "-c:a", "aac",
    "-f", "hls",
    "-hls_time", "2",
    "-hls_list_size", "5",
    "-hls_flags", "delete_segments",
    hlsFile
  ],
  { stdio: ["ignore", "pipe", "pipe"]});
  ffmpeg.stdout.on("data", d => console.log(`[ffmpeg out] ${d}`));
  ffmpeg.stderr.on("data", d => console.log(`[ffmpeg] ${d}`));
  ffmpeg.on("error", err => console.error("ffmpeg failed to start:", err));
  ffmpeg.on("close", code => console.log("ffmpeg exited with code", code));
  return ffmpeg;
}

function stopFFmpeg() {
  if (!ffmpegProc) return false;
  try {
    ffmpegProc.kill("SIGTERM");
  } catch {}
  ffmpegProc = null;
  return true;
}

const app = express();
app.use(cors());
app.use(express.static(hlsDir));

app.get("/status", (req, res) => {
  res.json({ running: !!ffmpegProc });
});

app.post("/stop", (req, res) => {
  const success = stopFFmpeg();
  res.json({ stopped });
})

app.post("/switch/:id", (req, res) => {
  const id = Number(req.params.id);
  const rtspUrl = rtspUrls[id];
  if (!rtspUrl) {
    return res.status(400).json({ error: "Unknown camera id" });
  }
  // 2) Stop any existing ffmpeg process (enforce single-stream)
  stopFFmpeg();
  // 3) Clear previous HLS files so the new playlist/segments are clean
  cleanHlsOutput();
  // 4) Start ffmpeg for the requested camera, writing to the same stream.m3u8
  ffmpegProc = startFFmpeg(rtspUrl);
  // 5) Respond with the playlist URL the frontend should load (always the same)
  res.json({ ok: true, m3u8: "/index.m3u8" });
});

app.listen(port, () => {
  console.log(`HLS server running at http://localhost:${port}`);
  console.log(`Playlist will be available at http://localhost:${port}/stream.m3u8`);
})

