import asyncio, logging, json
from fastapi import WebSocket
class WSHandler:
    def __init__(self, store):
        self.store = store
        self.clients = set()
        self.q = asyncio.Queue()
        self.log = logging.getLogger('WS')

    async def endpoint(self, websocket: WebSocket):
        await websocket.accept()
        self.clients.add(websocket)
        try:
            while True:
                await websocket.receive_text()  # ignore incoming
        except:
            pass
        finally:
            self.clients.remove(websocket)

    async def broadcaster(self):
        latest_seen = None
        while True:
            pkt = self.store.get_latest()
            if pkt and pkt is not latest_seen:
                latest_seen = pkt
                dead = []
                for ws in self.clients:
                    try:
                        await ws.send_json(pkt)
                    except:
                        dead.append(ws)
                for ws in dead: self.clients.remove(ws)
            await asyncio.sleep(0.2)
