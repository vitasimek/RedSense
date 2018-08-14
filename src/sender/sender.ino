#define F_CPU 4000000UL

#include <IRremote.h>
#include <boarddefs.h>
#include <IRremoteInt.h>
#include <avr/delay.h>
#include "data.h"

#define IR_LED 1
#define ANALOG_PWR 3
#define ANALOG_PIN A2

IRsend sender;

void setup()
{
	OSCCAL = 147;

	ADMUX = _BV(MUX3) | _BV(MUX2); // For ATTiny85

	pinMode(ANALOG_PWR, OUTPUT);
	digitalWrite(ANALOG_PWR, LOW);

	sender.enableIROut(38);
}

data_t data = {0xA};

void loop()
{
	// TODO: Optimize with different type (data packet);
	data.checksum = 0;

	digitalWrite(ANALOG_PWR, HIGH);
	//	data.analog = analogRead(ANALOG_PIN);
	data.analog = 1023;
	digitalWrite(ANALOG_PWR, LOW);

	//	data.battery = readVcc() - 2800;
	data.battery = (4300 - 2800) / 100;

	data_serializer_t serializer;
	serializer.data = data;

	// TODO: Optimize with different type (data packet);
	data.checksum = computeChecksum(serializer.raw_data);
	serializer.data = data;

	sendMessage(serializer.raw_data);

	_delay_ms(1000);
}

void sendMessage(unsigned long data)
{
	sender.enableIROut(38);

	TIMER_ENABLE_PWM;
	_delay_us(2400);
	TIMER_DISABLE_PWM;
	_delay_us(600);

	//mark(2400);
	//space(600);

	for (unsigned long mask = 0x00000001UL << (DATA_LEN_BITS - 1); mask; mask >>= 1)
	{
		if (data & mask)
		{
			TIMER_ENABLE_PWM;
			_delay_us(1200);
			TIMER_DISABLE_PWM;
			_delay_us(600);
			//mark(1200);
			//space(600);
		}
		else
		{
			TIMER_ENABLE_PWM;
			_delay_us(600);
			TIMER_DISABLE_PWM;
			_delay_us(600);
			//mark(600);
			//space(600);
		}
	}
}

//void  sendRaw(const unsigned int buf[], unsigned int len, unsigned int hz)
//{
//	// Set IR carrier frequency
//	sender.enableIROut(hz);
//
//	for (unsigned int i = 0; i < len; i++) {
//		if (i & 1)  space(buf[i]);
//		else        mark(buf[i]);
//	}
//
//	space(0);  // Always end with the LED off
//}

////+=============================================================================
//// Sends an IR mark for the specified number of microseconds.
//// The mark output is modulated at the PWM frequency.
////
//void  mark(unsigned int time)
//{
//	TIMER_ENABLE_PWM; // Enable pin 3 PWM output
//	if (time > 0) _delay_us(time);
//}
//
////+=============================================================================
//// Leave pin off for time (given in microseconds)
//// Sends an IR space for the specified number of microseconds.
//// A space is no output, so the PWM output is disabled.
////
//void  space(unsigned int time)
//{
//	TIMER_DISABLE_PWM; // Disable pin 3 PWM output
//	if (time > 0) _delay_us(time);
//}

long readVcc()
{

	ADMUX = _BV(MUX3) | _BV(MUX2);

	_delay_ms(2);
	ADCSRA |= _BV(ADSC);
	while (bit_is_set(ADCSRA, ADSC))
		;

	uint8_t low = ADCL;
	uint8_t high = ADCH;

	long result = (high << 8) | low;

	result = 1125300L / result; // V�po�et Vcc (mV); 1125300 = 1.1*1023*1000
	return result;
}