typedef struct
{
  byte serial_no;
  unsigned int value;
  byte checksum;
} data_t;

typedef union
{
  data_t data;
  unsigned long raw_data;
} data_serializer_t;

