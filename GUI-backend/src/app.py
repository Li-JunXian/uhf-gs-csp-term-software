from flask import Flask, request, jsonify
from flask_socketio import SocketIO
import threading, time

from config import STATUS_FIFO, CSP_HOST, CSP_PORT, POLLING_INTERVAL
from state.datastore import TelemetryStore
from gs_link.gs_status import GSStatusReader
from gs_link.telemetry_server import TelemetryServer
from gs_link.command_client import CommandClient

app = Flask(__name__)
socketio = SocketIO(app)
store = TelemetryStore()
cmd_client = CommandClient(store)
store.enqueue_command = store.push

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
    command = request.json.get('command')
    if command:
        cmd_client.send(command)
        # Record the command in the telemetry store
        store.enqueue_command(command)
        return jsonify({"result": "Command sent"})

def broadcast_continous():
    last = None
    while True:
        pkt = store.get_latest()
        if pkt != last:
            socketio.emit('update', pkt)
            last = pkt
        time.sleep(POLLING_INTERVAL)

if __name__ == '__main__':
    GSStatusReader(store).start()
    TelemetryServer(store).start()
    cmd_client.start()
    
    socketio.start_background_task(broadcast_continous)
    socketio.run(app, host='0.0.0.0', port=8000)
