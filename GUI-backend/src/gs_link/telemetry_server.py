import socket, threading, struct, time, logging
from config import GS_TM_PORT
from utils.csp import parse_csp

FRAME_SIZE = 256  # Set this to the correct frame size for your application

def recvn(sock, n):
    data = b''
    while len(data) < n:
        packet = sock.recv(n - len(data))
        if not packet:
            return None
        data += packet
    return data

class TelemetryServer:
    def __init__(self, store, host='0.0.0.0', port=GS_TM_PORT):
        self.store, self.host, self.port = store, host, port
        self.log = logging.getLogger('TM-Server')

    def start(self):
        t = threading.Thread(target=self._run, daemon=True)
        t.start()

    def _run(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.bind((self.host, self.port))
        sock.listen(1)
        conn, _ = sock.accept()
        while True:
            # read exactly one full frame
            frame = recvn(conn, FRAME_SIZE)
            if frame is None:
                break
            pkt = parse_csp(frame)
            pkt['timestamp'] = time.time()
            self.store.push(pkt)
