#include <Arduino.h>

#define DATA_LEN sizeof(data_t)
#define DATA_LEN_BITS DATA_LEN * 8

typedef struct
{
  byte serial_no : 4;
  unsigned int analog : 10;
  unsigned int battery : 4; // 2.8V - 4.3V  1024 < 1600 < 2048 (11bit)
  boolean button_1 : 1;
  boolean button_2 : 1;
  byte checksum : 4;
} data_t;

typedef union {
  data_t data;
  unsigned long raw_data;
} data_serializer_t;

byte computeChecksum(unsigned long raw_data)
{

  byte table[] = {
      0x0, 0x7, 0xe, 0x9, 0xb, 0xc, 0x5, 0x2,
      0x1, 0x6, 0xf, 0x8, 0xa, 0xd, 0x4, 0x3
  };

  byte crc = 0;

  crc = table[crc ^ ((raw_data & 0x00000F) >> 0x00)];
  crc = table[crc ^ ((raw_data & 0x0000F0) >> 0x04)];
  crc = table[crc ^ ((raw_data & 0x000F00) >> 0x08)];
  crc = table[crc ^ ((raw_data & 0x00F000) >> 0x0C)];
  crc = table[crc ^ ((raw_data & 0x0F0000) >> 0x10)];
  crc = table[crc ^ ((raw_data & 0xF00000) >> 0x14)];

  return crc;
}
