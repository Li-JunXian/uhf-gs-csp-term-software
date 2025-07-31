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

def kiss_deframe(stream: bytearray) -> list[bytes]:
    """Consume *stream* in-place; return list of whole DATA frames found."""
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

# ---------- CSP header (6-byte disk format) ----------
def unpack_header(h: bytes) -> dict:
    """Return dict with prio, src, dst, dport, sport, length, flag."""
    if len(h) != 7:
        raise ValueError("expect 7-byte header")
    id_lo2 = h[0] | (h[1] << 8)
    prio  = (id_lo2 >> 13) & 0x07          # top 3 bits
    src   = (id_lo2 >> 8)  & 0x1F
    dst   =  id_lo2        & 0x1F
    dport = h[2] >> 2
    sport = ((h[2] & 0x03) << 4) | (h[3] >> 4)
    length = (h[4] << 8) | h[5]
    # extract new flag field
    flag = h[6]
    return dict(
        prio=prio,
        src=src,
        dst=dst,
        dport=dport,
        sport=sport,
        length=length,
        flag=flag
    )
