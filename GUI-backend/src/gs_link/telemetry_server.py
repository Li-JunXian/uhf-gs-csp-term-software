import socket, threading, time, logging
from config import CSP_HOST, CSP_PORT
from utils.csp import unpack_header

FRAME_SIZE = 256  # keep this unless you switch to variable-length reads

def recvn(conn, n):
    data = b''
    while len(data) < n:
        chunk = conn.recv(n - len(data))
        if not chunk:
            return None
        data += chunk
    return data

class TelemetryServer(object):
    def __init__(self, store, host=CSP_HOST, port=CSP_PORT):
        self.store = store
        self.host = host
        self.port = port
        self.log = logging.getLogger('TM-Server')

    def start(self):
        t = threading.Thread(target=self._run)
        t.daemon = True
        t.start()

    def _run(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((self.host, self.port))
        s.listen(1)
        self.log.info('Telemetry listening on %s:%s', self.host, self.port)
        while True:
            conn, addr = s.accept()
            self.log.info('Telemetry connection from %s', addr[0])
            try:
                while True:
                    frame = recvn(conn, FRAME_SIZE)
                    if frame is None:
                        break
                    hdr = unpack_header(frame)
                    payload = frame[hdr.get('_header_len', 0):]
                    # hex for JSON safety on Py3.5
                    if hasattr(payload, 'hex'):
                        payload_hex = payload.hex()
                    else:
                        payload_hex = ''.join('{:02x}'.format(b) for b in payload)

                    env = {
                        'type': 'telemetry',
                        'ts': time.time(),
                        'data': dict(hdr, payload=payload_hex, length=len(payload))
                    }
                    self.store.push({'telemetry': env})
            except Exception as e:
                self.log.warning('TM link error: %r', e)
            finally:
                try:
                    conn.close()
                except Exception:
                    pass
