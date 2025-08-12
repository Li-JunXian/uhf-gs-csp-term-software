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
        threading.Thread(target=self._run, name='GS-Status', daemon=True).start()

    # Signal the reader thread to stop.
    def stop(self):
        self.running = False

    def _run(self):
        if not os.path.exists(STATUS_FIFO):
            os.mkfifo(STATUS_FIFO, 0o666)
        with open(STATUS_FIFO, "r") as fifo:
            
            # FIFO opened, send initial status
            pkt = {"type": "gs_status", "event": "status_fifo_opened"}
            
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
                except ValueError as e:
                    log.error("Malformed JSON in FIFO: {0} -- {1!r}".format(e, line))
                    continue

                # Append timestamp and push into the shared store
                pkt["timestamp"] = time.time()
                self.store.push(pkt)
                log.debug("GS Status update: {0}".format(pkt))
