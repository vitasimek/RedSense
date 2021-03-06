#include <boarddefs.h>
#include <IRremote.h>
#include "data.h"

#define CONTROL 2
#define BUTTON_1 5
#define BUTTON_2 4
#define BUTTON_3 3
#define LED 13
#define SPEAKER 6

IRrecv receiver(CONTROL, LED);

void setup()
{
  Serial.begin(115200);

  receiver.blink13(true);
  receiver.enableIRIn();
}

void loop()
{
  decode_results results;

  if (receiver.decode(&results))
  {
    decodeMessage(&results);

    data_serializer_t serializer;
    serializer.raw_data = results.value;

    byte actualChecksum = serializer.data.checksum;
    serializer.data.checksum = 0;

    byte expectedChecksum = computeChecksum(serializer.raw_data);
    bool isValid = (actualChecksum == expectedChecksum);

    if (isValid)
    {
      Serial.print("Type: 0x");
      Serial.print(results.decode_type, HEX);
      Serial.print("\t");
      Serial.print("Raw length: ");
      Serial.print(results.rawlen);
      Serial.print("\t");
      Serial.print("S/N: 0x");
      Serial.print(serializer.data.serial_no, HEX);
      Serial.print("\t");
      Serial.print("Battery: ");
      Serial.print(serializer.data.battery * 100 + 2800);
      Serial.print("\t");
      Serial.print("Analog: ");
      Serial.print(serializer.data.analog);
      Serial.println();
    }
    else
    {
      Serial.println(results.rawlen);
    }

    receiver.resume();
  }
}

bool decodeMessage(decode_results *results)
{
  long data = 0;
  int offset = 0; // Dont skip first space, check its size

  if (irparams.rawlen < (2 * DATA_LEN_BITS) + 2)
  {
    return false;
  }

  offset++;

  if (!MATCH_MARK(results->rawbuf[offset++], 2400))
  {
    return false;
  }

  while (offset + 1 < irparams.rawlen)
  {
    if (!MATCH_SPACE(results->rawbuf[offset++], 600))
    {
      break;
    }

    if (MATCH_MARK(results->rawbuf[offset], 1200))
    {
      data = (data << 1) | 1;
    }
    else if (MATCH_MARK(results->rawbuf[offset], 600))
    {
      data = (data << 1) | 0;
    }
    else
    {
      return false;
    }

    offset++;
  }

  results->bits = (offset - 1) / 2;
  if (results->bits < DATA_LEN_BITS)
  {
    results->bits = 0;
    return false;
  }
  results->value = data;
  results->decode_type = 0x49;
  return true;
}
