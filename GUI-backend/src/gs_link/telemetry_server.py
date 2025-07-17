import socket, threading, struct, time, logging
from config import GS_TM_PORT
from utils.csp import unpack_header

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
        s = socket.socket()
        s.bind((self.host, self.port))
        s.listen(1)
        self.log.info(f'Telemetry server listening @ {self.port}')
        conn, addr = s.accept()
        self.log.info(f'GS connected from {addr}')
        try:
            while True:
                header = recvn(conn, 6)
                if not header:
                    break
                hdr = unpack_header(header)
                pri    = hdr['prio']
                src    = hdr['src']
                dst    = hdr['dst']
                dport  = hdr['dport']
                sport  = hdr['sport']
                length = hdr['length']
                payload = conn.recv(length)
                pkt = {
                    "timestamp": time.time(),
                    "src": src, "dst": dst,
                    "src_port": sport, "dst_port": dport,
                    "payload": payload.hex()
                }
                self.store.push(pkt)
                self.log.info(f'Pkt {pkt}')
                
        except Exception as e:
            self.log.error(f'Telemetry server error: {e}')
        finally:
            conn.close()
            s.close()
            self.log.info('Telemetry server closed')
