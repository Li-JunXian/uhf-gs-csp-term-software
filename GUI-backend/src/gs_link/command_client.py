import socket
import threading
import queue
import logging
import time
from config import GS_CMD_HOST, GS_CMD_PORT
from utils.csp import unpack_header

class CommandClient:
    def __init__(self, store, host=GS_CMD_HOST, port=GS_CMD_PORT):
        """Initialize with the TelemetryStore, then start the command loop."""
        self.store = store
        self.q = queue.Queue()
        self.host = host
        self.port = port
        self.log = logging.getLogger('CMD-Client')
        # start the command‐processor thread in the background
        threading.Thread(target=self._run, daemon=True).start()

    def send(self, raw_bytes: bytes):
        self.q.put(raw_bytes)

    def start(self):
        t = threading.Thread(target=self._run, daemon=True)
        t.start()

    def _run(self):
        while True:
            try:
                sock = socket.create_connection((self.host, self.port))
                self.log.info('Connected to GS CMD port')
                while True:
                    data = self.q.get()
                    sock.sendall(data)
            except (OSError, ConnectionResetError):
                self.log.warning('GS CMD link lost, retrying …')
                time.sleep(2)
