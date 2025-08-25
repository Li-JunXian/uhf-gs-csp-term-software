#ifndef CRC16_H
#define CRC16_H

#include <stdint.h>
#include <stddef.h>

uint16_t crc16(const uint8_t *data, size_t len);
/* Variant: CRC-16/CCITT-FALSE, poly 0x1021, init 0xFFFF, no final XOR.
   If your link expects a different variant (X25/KERMIT), we can switch. */

#endif
