import threading
import queue
from collections import defaultdict, deque

class TelemetryStore(object):
  
    def __init__(self, hist_len=1000):
        self._lock = threading.Lock()
        self._latest = {}                                  # topic -> last envelope
        self._hist   = defaultdict(lambda: deque(maxlen=hist_len))
        self._event_q = queue.Queue()                      # (topic, envelope) for WS

    def push(self, typed_pkt):
        # typed_pkt is a dict with exactly one item: {topic: envelope}
        topic, env = next(iter(typed_pkt.items()))
        with self._lock:
            self._latest[topic] = env
            self._hist[topic].append(env)
        self._event_q.put((topic, env))

    def get_latest(self):
        with self._lock:
            return dict(self._latest)

    def get_latest_status(self):
        env = self._latest.get('gs_status')
        if not env:
            return {}
        # Envelope: {"type":"gs_status","ts":...,"data":{...}}
        if isinstance(env, dict) and 'data' in env:
            return env['data']
        # Legacy/raw dict path
        return env

    def get_latest_telemetry(self):
        env = self._latest.get('telemetry')
        if not env:
            return {}
        if isinstance(env, dict) and 'data' in env:
            return env['data']
        return env

    # Generic helpers (handy for new REST endpoints / GUI):
    def get_topic(self, topic):
        return self._latest.get(topic, {})

    def get_history(self, topic, n=200):
        with self._lock:
            return list(self._hist[topic])[-n:]
