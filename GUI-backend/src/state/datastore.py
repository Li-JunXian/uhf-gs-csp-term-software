import threading, time
class TelemetryStore:
    def __init__(self):
        self._lock = threading.Lock()
        self.latest = None
        self.history = []

    def push(self, pkt: dict):
        with self._lock:
            self.latest = pkt
            self.history.append(pkt)

    def get_latest(self):
        with self._lock:
            return self.latest
