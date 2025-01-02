// Program that allows the user to input a raw hex value from a remote to change the color of an LED string.

#include "FastLED.h"
#include "IRremote.h"
#include <sstream>

#define NUM_LEDS 200
#define DATA_PIN 5
#define IR_PIN 25
#define CHIPSET WS2812
#define RGB_ORDER RGB

CRGB leds[NUM_LEDS];
std::string StringBuffer;

void setup()
{
	Serial.begin( 9600 );
	delay( 2000 );
	pinMode( IR_PIN, INPUT );
	FastLED.addLeds<CHIPSET, DATA_PIN, RGB_ORDER>( leds, NUM_LEDS );
	FastLED.clear( true );
	FastLED.setBrightness( 50 );
	IrReceiver.begin( IR_PIN );
}

void AddChar( char character )
{
	StringBuffer += character;
	Serial.print( "Adding character: " );
	Serial.println( character );
}

void loop()
{
	if ( IrReceiver.decode() )
	{
		auto data = IrReceiver.decodedIRData.decodedRawData;
		switch ( data )
		{
			case 0x1051: case 0x11051: // Remote can send out either of these for one button; they alternate after every press
				AddChar( 'A' );
				break;
			case 0x10B9: case 0x110B9:
				AddChar( 'B' );
				break;
			case 0x44B: case 0x1044B:
				AddChar( 'C' );
				break;
			case 0x44E: case 0x1044E:
				AddChar( 'D' );
				break;
			case 0x4F7: case 0x104F7:
				AddChar( 'E' );
				break;
			case 0x41D: case 0x1041D:
				AddChar( 'F' );
				break;
			case 0x401: case 0x10401:
				AddChar( '1' );
				break;
			case 0x402: case 0x10402:
				AddChar( '2' );
				break;
			case 0x403: case 0x10403:
				AddChar( '3' );
				break;
			case 0x404: case 0x10404:
				AddChar( '4' );
				break;
			case 0x405: case 0x10405:
				AddChar( '5' );
				break;
			case 0x406: case 0x10406:
				AddChar( '6' );
				break;
			case 0x407: case 0x10407:
				AddChar( '7' );
				break;
			case 0x408: case 0x10408:
				AddChar( '8' );
				break;
			case 0x409: case 0x10409:
				AddChar( '9' );
				break;
			case 0x400: case 0x10400:
				AddChar( '0' );
				break;
			case 0x45C: case 0x1045C:
				int hex;
				std::stringstream ss;
				ss << std::hex << StringBuffer;
				ss >> hex;
				Serial.print( "Final hex code: " );
				Serial.println( hex, HEX );
				FastLED.showColor( hex );
				StringBuffer.clear();
				break;
		}
		delay( 300 ); // Small delay so only the first button press is registered
	}
	IrReceiver.resume();
}
