import asyncio
from collections import deque
from typing import Any, Dict, List
class TelemetryStore:
    def __init__(self, maxlen: int = 100):
        self.buffer: deque[Dict[str, Any]] = deque(maxlen=maxlen)
        self.queue: asyncio.Queue[Dict[str, Any]] = asyncio.Queue()

    def push(self, pkt: Dict[str, Any]) -> None:
        self.buffer.append(pkt)
        self.queue.put_nowait(pkt)

    def get_latest(self, limit: int = 10) -> List[Dict[str, Any]]:
        item = list(self.buffer)
        return item[-limit:]
    
    async def subscribe(self):
        while True:
            pkt = await self.queue.get()
            yield pkt
            self.queue.task_done()
