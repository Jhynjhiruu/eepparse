#pragma once

#include <stdint.h>

typedef struct
{
    char code[4];
    char gamename[12];
    uint16_t checksum; // little-endian
} __attribute__ ((packed)) SaveHeader;

typedef struct
{
    char magic[4];
    SaveHeader headers[6];
    uint8_t padding[18];
    uint8_t saves[0x500][6];
    uint8_t misc[0x17E];
} __attribute__ ((packed)) Eeprom;
