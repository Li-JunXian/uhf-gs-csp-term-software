import express from "express"
import { spawn } from "child_process";
import path from "path";
import { fileURLToPath } from "url";
import cors from "cors"
import fs from "fs"
import { start } from "repl";
import { error } from "console";
import * as cheerio from 'cheerio'



const port = 4000;

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

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

app.get('/api/satellites', async(req, res) => {
  try {
    //URL of the NUSH satellite being tracked convert into a list to track additional satellites
    const urls = [
      'https://db.satnogs.org/satellite/AGMX-2508-1337-4945-9939#data',
      'https://db.satnogs.org/satellite/ARJF-4282-1922-6487-5841#data'
    ];
    const out = [];
    for (const url of urls) {
      const response = await fetch(url);

      const $ = cheerio.load(await response.text());
      let name = $('span.h4.mb-0:eq(1)').text()
      let lastUpdate = new Date($('dd.col-sm-10:eq(1)').text().trim());
      let TLE = ($('.tle-set').html()).replace("<br>", "\n");
      let TLE1 = TLE.substring(0, TLE.indexOf('\n'));
      let TLE2 = TLE.substring(TLE.indexOf('\n') + 1);
      out.push({name: name, TLE1: TLE1, TLE2: TLE2, lastUpdate: lastUpdate});
    }
    res.json(out)
  } catch (err) {
    console.error(err)
    res.status(400).json({ error: 'Failed to fetch satellite data' });
  }
})

app.post("/switch/:id", (req, res) => {
  const id = Number(req.params.id);
  const rtspUrl = rtspUrls[id];
  if (!rtspUrl) {
    return res.status(404).json({ error: "Unknown camera id" });
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

