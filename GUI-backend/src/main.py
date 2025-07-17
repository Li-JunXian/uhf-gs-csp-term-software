import logging, uvicorn, asyncio
from api.rest                 import api
import api.rest               as rest_mod
from gs_link.telemetry_server import TelemetryServer
from gs_link.command_client   import CommandClient
from state.datastore          import TelemetryStore
from api.ws                   import WSHandler
from config                   import API_PORT, LOG_LEVEL

logging.basicConfig(level=LOG_LEVEL)

store = TelemetryStore()

# inject store into REST & WS modules
rest_mod.store = store
ws_handler = WSHandler(store)

TelemetryServer(store).start()
cmd_client = CommandClient()          # creates thread + queue

# mount WS route and run FastAPI
api.add_api_websocket_route("/stream", ws_handler.endpoint)
asyncio.get_event_loop().create_task(ws_handler.broadcaster())

if __name__ == "__main__":
    uvicorn.run(api, host="0.0.0.0", port=API_PORT)