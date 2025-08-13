import os, json, time, logging, threading

from config import STATUS_FIFO

class GSStatusReader(object):
    def __init__(self, store):
        self.store = store
        self.log = logging.getLogger('GS-Status')

    def start(self):
        threading.Thread(target=self._run, name='GS-Status', daemon=True).start()

    def __init__(self, store):
        self.running = True
        self.store = store
        self.log = logging.getLogger('GS-Status')

    def start(self):
        t = threading.Thread(target=self._run)
        t.daemon = True
        t.start()

    def _run(self):
        # Ensure FIFO exists
        if not os.path.exists(STATUS_FIFO):
            try:
                os.mkfifo(STATUS_FIFO, 0o666)
            except OSError:
                pass

        with open(STATUS_FIFO, "r") as fifo:
            while self.running:
                line = fifo.readline()
                if not line:
                    time.sleep(0.05)
                    continue
                line = line.strip()
                if not line:
                    continue
                try:
                    pkt = json.loads(line)
                except ValueError as e:
                    self.log.error("Malformed JSON in FIFO: %s -- %r", e, line)
                    continue

                ts = pkt.get('ts', time.time())
                # Envelope from C: {"type": "<topic>", "ts": ..., "data": {...}}
                if isinstance(pkt, dict) and 'type' in pkt and 'data' in pkt:
                    topic = pkt['type']
                    env = {"type": topic, "ts": ts, "data": pkt['data']}
                    # store envelope as <topic>
                    self.store.push({topic: env})
                    # additionally keep a leaf for the classic /status route
                    if topic == 'gs_status':
                        self.store.push({'gs_status': env})
                else:
                    # Legacy plain object => treat as gs_status leaf + envelope
                    env = {"type": "gs_status", "ts": ts, "data": pkt}
                    self.store.push({'gs_status': env})
                