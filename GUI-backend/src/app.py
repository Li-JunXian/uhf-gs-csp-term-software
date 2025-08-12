from flask import Flask, request, jsonify
from flask_socketio import SocketIO
import threading, time, logging

from config import STATUS_FIFO, CSP_HOST, CSP_PORT, POLLING_INTERVAL, API_HOST, API_PORT, LOG_LEVEL
from state.datastore import TelemetryStore
from gs_link.gs_status import GSStatusReader
from gs_link.telemetry_server import TelemetryServer
from gs_link.command_client import CommandClient

app = Flask(__name__)
socketio = SocketIO(app, async_mode='eventlet')
store = TelemetryStore()
cmd_client = CommandClient(store)

@app.route('/status')
def get_status():
    data = store.get_latest_status()
    return jsonify(data)

@app.route('/telemetry')
def get_telemetry():
    data = store.get_latest_telemetry()
    return jsonify(data)

@app.route('/command', methods=['POST'])
def post_command():
    payload = request.json.get('command')
    if not payload:
        return jsonify({"error": "missing 'command'"}), 400
    # Ensure bytes for CommandClient.send(...)
    try:
        raw = payload.encode('ascii')
    except Exception:
        raw = payload.encode('utf-8')
    cmd_client.send(raw)
    # Record the command event in the store for UI/WS
    store.push({"type": "command", "value": payload, "timestamp": time.time()})
    return jsonify({"result": "queued"})

def broadcast_continuous():
    last = None
    while True:
        pkt = store.get_latest()
        if pkt != last:
            socketio.emit('update', pkt)
            last = pkt
        time.sleep(POLLING_INTERVAL)

if __name__ == '__main__':
    print("Starting Flask/SocketIO on {}:{}".format(API_HOST, API_PORT))
    logging.getLogger().setLevel(getattr(logging, LOG_LEVEL, logging.INFO))
    GSStatusReader(store).start()
    TelemetryServer(store).start()
    cmd_client.start()
    
    socketio.start_background_task(broadcast_continuous)
    socketio.run(app, host=API_HOST, port=API_PORT)
