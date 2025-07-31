import threading
import queue
from collections import deque
from typing import Any, Dict, List
class TelemetryStore:
    def __init__(self):
        self._lock = threading.Lock()
        self._data = {}
        self._event_q = queue.Queue()

    def push(self, pkt):
        with self._lock:
            self._data.update(pkt)
        self._event_q.put(pkt)

    def get_latest(self):
        with self._lock:
            return self._data.copy()
    
    def get_latest_status(self):
        return self.get_latest().get('gs_status', {})
    
    def get_latest_telemetry(self):
        return self.get_latest().get('telemetry', {})
    