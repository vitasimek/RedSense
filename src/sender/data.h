#include <Arduino.h>

#define DATA_LEN sizeof(data_t)
#define DATA_LEN_BITS DATA_LEN * 8

typedef struct
{
  byte serial_no : 4;
  unsigned int analog : 10;
  unsigned int battery : 4;   // 2.8V - 4.3V  1024 < 1600 < 2048 (11bit)
  boolean button_1 : 1;
  boolean button_2 : 1;
  byte checksum : 4;
} data_t;

typedef union
{
  data_t data;
  unsigned long raw_data;
} data_serializer_t;

