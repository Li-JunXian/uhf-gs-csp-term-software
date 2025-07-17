import socket, threading, queue, logging, time
from config import GS_CMD_PORT
from utils.csp import unpack_header
class CommandClient:
    def __init__(self, host='127.0.0.1', port=GS_CMD_PORT):
        self.q = queue.Queue()
        self.host, self.port = host, port
        self.log = logging.getLogger('CMD-Client')
        threading.Thread(target=self._run, daemon=True).start()

    def send(self, raw_bytes: bytes):
        self.q.put(raw_bytes)

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
