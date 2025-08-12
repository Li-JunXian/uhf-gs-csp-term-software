# GUI-backend/src/gs_link/command_client.py
from __future__ import print_function
import socket
import threading
import queue
import logging
import time
from config import GS_CMD_HOST, GS_CMD_PORT

class CommandClient(object):
    def __init__(self, host=GS_CMD_HOST, port=GS_CMD_PORT):
        self.host = host
        self.port = port
        self.q = queue.Queue()
        self.log = logging.getLogger('CMD-Client')
        self._stop = False
        t = threading.Thread(target=self._run, name='CMD-Client')
        t.daemon = True
        t.start()

    def send(self, raw_bytes):
        """Enqueue a bytes payload to send to the GS TCP command port."""
        if not isinstance(raw_bytes, (bytes, bytearray)):
            raise TypeError('CommandClient.send expects bytes')
        # copy to immutable bytes to be safe
        self.q.put(bytes(raw_bytes))

    def _run(self):
        sock = None
        while not self._stop:
            try:
                if sock is None:
                    sock = socket.create_connection((self.host, self.port), timeout=5)
                    self.log.info('Connected to GS CMD at %s:%s', self.host, self.port)
                data = self.q.get()  # blocks until something to send
                if not data:
                    continue
                sock.sendall(data)
            except Exception as e:
                self.log.warning('GS CMD link lost (%r), retrying …', e)
                try:
                    if sock:
                        sock.close()
                except Exception:
                    pass
                sock = None
                time.sleep(2)
