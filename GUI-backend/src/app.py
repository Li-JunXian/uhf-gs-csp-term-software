from flask import Flask, request, jsonify
from flask_socketio import SocketIO
import threading, time, logging
import base64
import struct

from config import STATUS_FIFO, CSP_HOST, CSP_PORT, POLLING_INTERVAL, API_HOST, API_PORT, LOG_LEVEL, GS_CMD_HOST, GS_CMD_PORT
from state.datastore import TelemetryStore
from gs_link.gs_status import GSStatusReader
from gs_link.telemetry_server import TelemetryServer

from gs_link.command_client import CommandClient

def hex_to_bytes(s):
    """Python 3.5-safe hex parser: accepts strings like 'AABB' or 'AA BB'."""
    if s is None:
        raise ValueError('hex string is None')
    # remove whitespace/newlines
    s = s.replace(' ', '').replace('\n', '').replace('\r', '')
    if len(s) % 2 != 0:
        raise ValueError('hex string must have even length')
    out = []
    for i in range(0, len(s), 2):
        out.append(int(s[i:i+2], 16))
    return bytes(bytearray(out))

app = Flask(__name__)
socketio = SocketIO(app, async_mode='threading')
store = TelemetryStore()
cmd_client = CommandClient(host=GS_CMD_HOST, port=GS_CMD_PORT)

@app.route('/status')
def get_status():
    data = store.get_latest_status()
    return jsonify(data)

@app.route('/telemetry')
def get_telemetry():
    data = store.get_latest_telemetry()
    return jsonify(data)

@app.route('/latest/<topic>')
def latest_topic(topic):
    env = store.get_topic(topic)
    return jsonify(env or {})

@app.route('/history/<topic>')
def history_topic(topic):
    try:
        n = int(request.args.get('n', 200))
    except Exception:
        n = 200
    return jsonify(store.get_history(topic, n))

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

@app.route('/health')
def health():
    latest = store.get_latest()
    hb = latest.get('health', {})
    age = time.time() - hb.get('ts', 0)
    return jsonify({"ok": age < 5, "age_sec": age, "topics": list(latest.keys())})

@app.route('/topics')
def topics():
    return jsonify(sorted(store.get_latest().keys()))

def broadcast_continuous():
    while True:
        topic, env = store._event_q.get()
        socketio.emit('update', {'topic': topic, 'event': env})

@app.route('/cmd/raw', methods=['POST'])
def cmd_raw():
    """
    Body can be:
      {"hex": "AABBCCDD"}   or
      {"base64": "qrs="}
    Sends raw bytes to GS CMD TCP port.
    """
    payload = request.get_json(force=True) or {}
    data = None
    if 'hex' in payload and payload['hex']:
        try:
            data = hex_to_bytes(payload['hex'])
        except Exception as e:
            return jsonify({"ok": False, "error": "bad hex: %s" % (e,)}), 400
    elif 'base64' in payload and payload['base64']:
        try:
            data = base64.b64decode(payload['base64'])
        except Exception as e:
            return jsonify({"ok": False, "error": "bad base64: %s" % (e,)}), 400
    else:
        return jsonify({"ok": False, "error": "provide 'hex' or 'base64'"}), 400

    try:
        cmd_client.send(data)
    except Exception as e:
        return jsonify({"ok": False, "error": str(e)}), 500
    return jsonify({"ok": True, "sent_bytes": len(data)})

@app.route('/cmd/csp', methods=['POST'])
def cmd_csp():
    """
    Optional convenience: build a simple CSP frame from fields,
    e.g. {"prio":0, "src":1, "dst":10, "sport":5, "dport":15, "flags":1, "payload_hex":"AABB"}
    Adjust to match your utils.csp packer if you have one.
    """
    payload = request.get_json(force=True) or {}
    # Very simple 8-byte header example; replace with your real packer if available.
    try:
        prio  = int(payload.get('prio', 0)) & 0x07
        src   = int(payload.get('src',  0)) & 0x1F
        dst   = int(payload.get('dst',  0)) & 0xFF
        sport = int(payload.get('sport',0)) & 0xFF
        dport = int(payload.get('dport',0)) & 0xFF
        flags = int(payload.get('flags',0)) & 0xFFFF
        plhex = payload.get('payload_hex', '') or ''
        body  = hex_to_bytes(plhex)

        # Example header layout (adjust to match your on-air format):
        # byte0: prio (3 bits) | src (5 bits)
        b0 = ((prio & 0x07) << 5) | (src & 0x1F)
        b1 = dst & 0xFF
        b2 = sport & 0xFF
        b3 = dport & 0xFF
        # length: two bytes big-endian (body length)
        length = len(body)
        b4 = (length >> 8) & 0xFF
        b5 = length & 0xFF
        b6 = (flags >> 8) & 0xFF
        b7 = flags & 0xFF

        frame = bytes(bytearray([b0,b1,b2,b3,b4,b5,b6,b7])) + body
    except Exception as e:
        return jsonify({"ok": False, "error": "bad CSP fields: %s" % (e,)}), 400

    try:
        cmd_client.send(frame)
    except Exception as e:
        return jsonify({"ok": False, "error": str(e)}), 500
    return jsonify({"ok": True, "sent_bytes": len(frame)})

if __name__ == '__main__':
    print("Starting Flask/SocketIO on {}:{}".format(API_HOST, API_PORT))
    logging.getLogger().setLevel(getattr(logging, LOG_LEVEL, logging.INFO))
    GSStatusReader(store).start()
    TelemetryServer(store).start()
    
    socketio.start_background_task(broadcast_continuous)
    socketio.run(app, host=API_HOST, port=API_PORT, debug=False, use_reloader=False)
