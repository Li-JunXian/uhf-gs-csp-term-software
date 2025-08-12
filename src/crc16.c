#include "crc16.h"

uint16_t crc16(const uint8_t *data, size_t len) {
    uint16_t crc = 0xFFFF;          // CCITT-FALSE init
    for (size_t i = 0; i < len; ++i) {
        crc ^= (uint16_t)data[i] << 8;
        for (int b = 0; b < 8; ++b) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;  // poly 0x1021
            else
                crc <<= 1;
        }
    }
    return crc;                      // no final XOR
}
