import socket, threading, time, logging
from config import GS_TM_PORT
from utils.csp import unpack_header

FRAME_SIZE = 256

def recvn(sock, n):
    data = b''
    while len(data) < n:
        chunk = sock.recv(n - len(data))
        if not chunk:
            return None
        data += chunk
    return data

class TelemetryServer(object):
    def __init__(self, store, host='127.0.0.1', port=GS_TM_PORT):
        self.store = store
        self.host = host
        self.port = port
        self.log = logging.getLogger('TelemetryServer')

    def start(self):
        t = threading.Thread(target=self._run, name='TelemetryServer', daemon=True)
        t.start()

    def _run(self):
        srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        srv.bind((self.host, self.port))
        srv.listen(1)
        self.log.info('Listening on %s:%d', self.host, self.port)

        while True:
            try:
                conn, addr = srv.accept()
                self.log.info('TM client connected from %s:%s', addr[0], addr[1])
                while True:
                    frame = recvn(conn, FRAME_SIZE)
                    if frame is None:
                        break
                    pkt = unpack_header(frame)
                    pkt['timestamp'] = time.time()
                    # tag so your store can disambiguate
                    self.store.push({'telemetry': pkt})
            except Exception as e:
                self.log.warning('TM link dropped: %r', e)
                try:
                    conn.close()
                except Exception:
                    pass
                time.sleep(0.5)
                continue
            