from typing import List
import struct

# ---------- KISS ----------
FEND, FESC, TFEND, TFESC = 0xC0, 0xDB, 0xDC, 0xDD

def kiss_escape(payload: bytes) -> bytes:
    out = bytearray()
    for b in payload:
        if b == FEND:
            out += bytes([FESC, TFEND])
        elif b == FESC:
            out += bytes([FESC, TFESC])
        else:
            out.append(b)
    return bytes(out)

def kiss_frame(data: bytes) -> bytes:
    return bytes([FEND, 0x00]) + kiss_escape(data) + bytes([FEND])

def kiss_deframe(stream: bytearray) -> List[bytes]:
    frames, in_frame, esc = [], bytearray(), False
    while stream:
        b = stream.pop(0)
        if b == FEND:
            if frames or in_frame:
                # end of current frame
                if in_frame and frames is not None:
                    frames.append(bytes(in_frame[1:]))  # drop KISS cmd byte
                in_frame.clear()
            in_frame = bytearray()  # start new
        elif b == FESC:
            esc = True
        else:
            if esc:
                b = TFEND if b == TFEND else (TFESC if b == TFESC else b)
                esc = False
            in_frame.append(b)
    return frames

def unpack_header(frame):
    b = bytearray(frame)
    if len(b) < 7:
        raise ValueError("need >=7 bytes for header")

    prio  = (b[0] >> 5) & 0x07
    src   = b[0] & 0x1F
    dst   = b[1]
    sport = b[2]
    dport = b[3]
    length = ((b[4] << 8) | b[5]) & 0xFFFF

    if len(b) >= 8:
        flags = ((b[6] << 8) | b[7]) & 0xFFFF
        header_len = 8
    else:
        flags = b[6] & 0xFF
        header_len = 7

    return {
        'prio': prio, 'src': src, 'dst': dst,
        'sport': sport, 'dport': dport,
        'length': length, 'flags': flags,
        '_header_len': header_len
    }
