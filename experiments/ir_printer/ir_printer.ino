// Reads input from an IR remote and displays the raw data, protocol, and frequency of the input

#include "IRremote.h"

#define IR_PIN 8

// Names of protocols supported by IRremote
String Protocols[] = {
	"Unknown", "Pulse Distance", "Pulse Width", "Denon", "Dish",
	"JVC", "LG", "LG2", "NEC", "Panasonic",
	"Kaseikyo", "Kaseikyo JVC", "Kaseikyo Denon", "Kaseikyo Sharp", "Kaseikyo Mitsubishi",
	"RC5", "RC6", "Samsung", "Sharp", "Sony", "Onkyo",
	"Apple", "Bosewave", "Lego PF", "Magiquest", "Whynter"
};

// Frequencies of supported protocols
String Frequencies[] = {
	"0", "0", "0", "38", "0",
	"38", "38", "38", "38", "0",
	"37", "37", "37", "37", "37",
	"36", "36", "38", "0", "40", "0",
	"0", "38", "0", "0", "0"
};

void setup()
{
	Serial.begin( 115200 );
	pinMode( IR_PIN, INPUT );
	IrReceiver.begin( IR_PIN );
}

void loop()
{
	if ( IrReceiver.decode() )
	{
		Serial.print( "Address: 0x" );
		Serial.println( IrReceiver.decodedIRData.address, HEX );
		Serial.print( "Command: 0x" );
		Serial.println( IrReceiver.decodedIRData.command, HEX );
		Serial.print( "Raw Data: 0x" );
		Serial.println( IrReceiver.decodedIRData.decodedRawData, HEX );
		Serial.print( "Protocol: " );
		Serial.println( Protocols[IrReceiver.decodedIRData.protocol] );
		Serial.print( "Carrier Frequency: " );

		String freq = Frequencies[IrReceiver.decodedIRData.protocol];
		if ( freq == "0" )
			Serial.print( "Unknown, but most likely 38" );
		else
			Serial.print( freq );
		
		Serial.println( " Khz" );
		Serial.println();
	}
	IrReceiver.resume();
}
