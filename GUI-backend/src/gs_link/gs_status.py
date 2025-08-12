import os, json, time, logging, threading

from config import STATUS_FIFO

class GSStatusReader(object):
    def __init__(self, store):
        self.store = store
        self.log = logging.getLogger('GS-Status')

    def start(self):
        threading.Thread(target=self._run, name='GS-Status', daemon=True).start()

    def _run(self):
        while True:
            try:
                if not os.path.exists(STATUS_FIFO):
                    try:
                        os.mkfifo(STATUS_FIFO)
                    except OSError:
                        pass
                with open(STATUS_FIFO, 'r') as f:
                    self.log.info('Reading GS status from %s', STATUS_FIFO)
                    for line in f:
                        line = line.strip()
                        if not line:
                            continue
                        try:
                            pkt = json.loads(line)
                        except ValueError:
                            self.log.warning('Ignoring corrupt JSON: %r', line)
                            continue

                        # If already enveloped, route by type; else wrap as gs_status
                        if isinstance(pkt, dict) and 'type' in pkt and 'data' in pkt:
                            if 'ts' not in pkt:
                                pkt['ts'] = time.time()
                            topic = pkt['type']
                            self.store.push({topic: pkt})
                        else:
                            env = {
                                'type': 'gs_status',
                                'ts': time.time(),
                                'data': pkt
                            }
                            self.store.push({'gs_status': env})
            except Exception as e:
                self.log.warning('FIFO read error (%r); retrying in 0.5s', e)
                time.sleep(0.5)
