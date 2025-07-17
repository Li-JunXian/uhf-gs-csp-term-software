import os, threading, time, json, asyncio
from config import STATUS_FIFO    # define "/tmp/gs_status_fifo" here
import logging
from state.datastore import TelemetryStore

log = logging.getLogger('GS-Status')

# This module reads JSON status updates from a FIFO file and pushes them into the shared store.
class GSStatusReader:
    def __init__(self, store: TelemetryStore) -> None:
        self.running = True
        self.store = store

    def start(self):
        t = threading.Thread(target=self._run, daemon=True)
        t.start()

    # Signal the reader thread to stop.
    def stop(self):
        self.running = False

    def _run(self):
        if not os.path.exists(STATUS_FIFO):
            os.mkfifo(STATUS_FIFO, 0o666)
        with open(STATUS_FIFO, "r") as fifo:
            
            # FIFO opened, send initial status
            pkt = {"type": "gs_status", "event": "status_fifo_opened"}
            asyncio.get_event_loop().call_soon_threadsafe(self.store.push, pkt)

            while self.running:
                line = fifo.readline()
                if not line:
                    time.sleep(0.1)
                    continue
                line = line.strip()
                if not line:
                    continue
                try:
                    pkt = json.loads(line)
                    pkt["timestamp"] = time.time()
                    # push into the same store your TelemetryServer uses
                    asyncio.get_event_loop().call_soon_threadsafe(self.store.push, pkt)
                except json.JSONDecodeError as e:
                    log.warning(f"Ignoring corrupt JSON from FIFO: {e!r} -- {line!r}")
                    continue
                log.debug(f"GS Status update: {pkt}")
