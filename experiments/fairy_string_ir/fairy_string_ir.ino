// Program intended to mimick the functionality of the BrizLabs 200 LED fairy string.
// I've also added some extra colors that are set from a separate remote.

#include "FastLED.h"
#include "IRremote.h"

#define NUM_LEDS 200
#define DATA_PIN 5
#define IR_PIN 25
#define CHIPSET WS2812
#define RGB_ORDER RGB

CRGB leds[NUM_LEDS];
byte CurrentEffect = 0;
byte PreviousEffect = 0;
bool FullyOff = false;
enum Effects {
	Off = 1, // Start at 1 so red doesn't get set automatically
	SolidRed, SolidDarkOrange, SolidLightOrange, SolidYellow,
	SolidGreen, SolidDarkGreen, SolidLightGreen, SolidSeafoam,
	SolidBlue, SolidLightBlue, SolidCyan, SolidDarkBlue,
	SolidWhite, SolidPink, SolidMagenta, SolidPurple, Festive,
	Halloween, DualColorsLambda, RGBSectioned, ChristmasRandom,
	RedWhiteBlue
};

void setup()
{
	delay( 2000 );
	pinMode( IR_PIN, INPUT );
	FastLED.addLeds<CHIPSET, DATA_PIN, RGB_ORDER>( leds, NUM_LEDS );
	FastLED.clear( true );
	IrReceiver.begin( IR_PIN );
}

// Displays 2 colors
void DualColors( int color1, int color2 )
{
	for ( int i = 0; i < NUM_LEDS; i++ )
	{
		leds[i] = mod8( i, 2 ) == 0 ? color1 : color2;
	}
	FastLED.show();
}

// Festive sequence of 5 colors
void ShowFestive()
{
	int count = 0;
	CRGB colors[5] = { CRGB::Red, CRGB::Magenta, CRGB::Yellow, CRGB::Blue, CRGB::Green };
	for ( int i = 0; i < NUM_LEDS; i++ )
	{
		if ( count > 4 ) count = 0;
		leds[i] = colors[count];
		count++;
	}
	FastLED.show();
}

// Displays red, green, and blue in sections
void SectionedRGB()
{
	int div = NUM_LEDS / 3;
	for ( int i = 0; i < NUM_LEDS; i++ )
	{
		if ( i < div )
		{
			leds[i] = CRGB::Red;
		}
		else if ( i >= div && i <= div * 2 )
		{
			leds[i] = CRGB::Green;
		}
		else
		{
			leds[i] = CRGB::Blue;
		}
	}
	FastLED.show();
}

// Displays a random pattern of Christmas colors
void RandomChristmas()
{
	EVERY_N_SECONDS( 1 )
	{
		CRGB colors[3] = {
			CRGB::Red,
			CRGB::Green,
			CRGB::White
		};

		for ( int i = 0; i < NUM_LEDS; i++ )
		{
			leds[i] = colors[random8( 3 )];
		}

		FastLED.show();
	}
}

// Pattern of 3 red, 3 white, 3 blue
void SetRedWhiteBlue()
{
	int count = 0;
	CRGB colors[3] = { CRGB::Red, CRGB::Gray, CRGB::Blue };
	for ( int a = 0; a < NUM_LEDS; a += 3 )
	{
		if ( count > 2 ) count = 0;

		for ( int b = 0; b < 3; b++ )
		{
			leds[a + b] = colors[count];
		}

		count++;
	}
	FastLED.show();
}

void PlayCurrentEffect()
{
	if ( FullyOff ) return;
	switch ( CurrentEffect )
	{
		case Off:
			FastLED.clear( true );
			FullyOff = true;
			break;
		case SolidRed:
			FastLED.showColor( CRGB::Red );
			break;
		case SolidDarkOrange:
			FastLED.showColor( 0xCC4700 );
			break;
		case SolidLightOrange:
			FastLED.showColor( 0xFF5900 );
			break;
		case SolidYellow:
			FastLED.showColor( CRGB::Yellow );
			break;
		case SolidGreen:
			FastLED.showColor( CRGB::Green );
			break;
		case SolidDarkGreen:
			FastLED.showColor( CRGB::ForestGreen );
			break;
		case SolidLightGreen:
			FastLED.showColor( CRGB::Lime );
			break;
		case SolidSeafoam:
			FastLED.showColor( CRGB::MediumSpringGreen );
			break;
		case SolidBlue:
			FastLED.showColor( CRGB::Blue );
			break;
		case SolidLightBlue:
			FastLED.showColor( CRGB::DeepSkyBlue );
			break;
		case SolidCyan:
			FastLED.showColor( CRGB::Cyan );
			break;
		case SolidDarkBlue:
			FastLED.showColor( 0x000011 );
			break;
		case SolidWhite:
			FastLED.showColor( CRGB::White );
			break;
		case SolidPink:
			FastLED.showColor( CRGB::HotPink );
			break;
		case SolidMagenta:
			FastLED.showColor( CRGB::Magenta );
			break;
		case SolidPurple:
			FastLED.showColor( CRGB::Purple );
			break;
		case Festive:
			ShowFestive();
			break;
		case Halloween:
			DualColors( 0xFF5900, CRGB::Purple );
			break;
		case DualColorsLambda:
			DualColors( 0xFF5900, 0x000011 );
			break;
		case RGBSectioned:
			SectionedRGB();
			break;
		case ChristmasRandom:
			RandomChristmas();
			break;
		case RedWhiteBlue:
			SetRedWhiteBlue();
			break;
	}
}

void loop()
{
	if ( IrReceiver.decode() )
	{
		auto data = IrReceiver.decodedIRData.decodedRawData;
		switch ( data )
		{
			case 0xEF10FF00:
				CurrentEffect = PreviousEffect;
				FullyOff = false;
				break;
			case 0xFC03FF00:
				PreviousEffect = CurrentEffect;
				CurrentEffect = Off;
				break;
			case 0xE619FF00:
				CurrentEffect = SolidRed;
				break;
			case 0xE817FF00:
				CurrentEffect = SolidDarkOrange;
				break;
			case 0xBF40FF00:
				CurrentEffect = SolidLightOrange;
				break;
			case 0xF50AFF00:
				CurrentEffect = SolidYellow;
				break;
			case 0xE41BFF00:
				CurrentEffect = SolidGreen;
				break;
			case 0xED12FF00:
				CurrentEffect = SolidDarkGreen;
				break;
			case 0xB34CFF00:
				CurrentEffect = SolidLightGreen;
				break;
			case 0xE11EFF00:
				CurrentEffect = SolidSeafoam;
				break;
			case 0xEE11FF00:
				CurrentEffect = SolidBlue;
				break;
			case 0xE916FF00:
				CurrentEffect = SolidLightBlue;
				break;
			case 0xFB04FF00:
				CurrentEffect = SolidCyan;
				break;
			case 0xF10EFF00:
				CurrentEffect = SolidDarkBlue;
				break;
			case 0xEA15FF00:
				CurrentEffect = SolidWhite;
				break;
			case 0xB24DFF00:
				CurrentEffect = SolidPink;
				break;
			case 0xFF00FF00:
				CurrentEffect = SolidMagenta;
				break;
			case 0xE51AFF00:
				CurrentEffect = SolidPurple;
				break;
			case 0xF00FFF00:
				CurrentEffect = Festive;
				break;
			case 0xF30CFF00:
				CurrentEffect = Halloween;
				break;
			case 0x10401: case 0x401:
				CurrentEffect = DualColorsLambda;
				break;
			case 0x10402: case 0x402:
				CurrentEffect = RGBSectioned;
				break;
			case 0x10403: case 0x403:
				CurrentEffect = ChristmasRandom;
				break;
			case 0x10404: case 0x404:
				CurrentEffect = RedWhiteBlue;
				break;
			case 0xFD02FF00:
				FastLED.setBrightness( 50 );
				break;
			case 0xB748FF00:
				FastLED.setBrightness( 255 );
				break;
		}
		IrReceiver.resume();
	}
	PlayCurrentEffect();
}
