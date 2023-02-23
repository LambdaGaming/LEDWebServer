#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <FastLED.h>
#include <LuaWrapper.h>
#include <SPIFFS.h>
#include <WiFi.h>

#define NUM_LEDS 200
#define DATA_PIN 5
#define CHIPSET WS2812
#define RGB_ORDER RGB

CRGB leds[NUM_LEDS];
const char *ssid = "";
const char *password = "";
AsyncWebServer server( 80 );
LuaWrapper lua;
String LuaScript = "";
bool ScriptActive = false;
bool Debug = false;

static int SetColor( lua_State *lua_state )
{
  int index = luaL_checkinteger( lua_state, 1 );
  int color = luaL_checkinteger( lua_state, 2 );
  if ( index > NUM_LEDS - 1 || index < 0 )
  {
    luaL_error( lua_state, "LED index is out of range." );
    return 0;
  }
  leds[index] = color;
  return 0;
}

static int SetSolidColor( lua_State *lua_state )
{
  int color = luaL_checkinteger( lua_state, 1 );
  FastLED.showColor( color );
  return 0;
}

static int PushColors( lua_State *lua_state )
{
  FastLED.show();
  return 0;
}

static int LuaDelay( lua_State *lua_state )
{
  int time = luaL_checkinteger( lua_state, 1 );
  delay( time );
  return 0;
}

static int LuaMillis( lua_State *lua_state )
{
  lua_pushnumber( lua_state, ( lua_Number ) millis() );
  return 1;
}

static int LuaHeatColor( lua_State *lua_state )
{
  uint8_t value = ( uint8_t ) luaL_checkinteger( lua_state, 1 );
  lua_pushnumber( lua_state, ( lua_Number ) DeconstructCRGB( HeatColor( value ) ) );
  return 1;
}

static int FadeToBlackBy( lua_State *lua_state )
{
  uint8_t value = ( uint8_t ) luaL_checkinteger( lua_state, 1 );
  fadeToBlackBy( leds, NUM_LEDS, value );
  return 0;
}

static int SetBrightness( lua_State *lua_state )
{
  uint8_t value = ( uint8_t ) luaL_checkinteger( lua_state, 1 );
  FastLED.setBrightness( value );
  return 0;
}

static int SetColorHSV( lua_State *lua_state )
{
  int index = luaL_checkinteger( lua_state, 1 );
  uint8_t hue = ( uint8_t ) luaL_checkinteger( lua_state, 2 );
  uint8_t sat = ( uint8_t ) luaL_checkinteger( lua_state, 3 );
  uint8_t val = ( uint8_t ) luaL_checkinteger( lua_state, 4 );
  if ( index > NUM_LEDS - 1 || index < 0 )
  {
    luaL_error( lua_state, "LED index is out of range." );
    return 0;
  }
  leds[index] = CHSV( hue, sat, val );
  return 0;
}

int DeconstructCRGB( CRGB color )
{
  return ( ( color.r & 0xFF ) << 16 ) | ( ( color.g & 0xFF ) << 8 ) | ( color.b & 0xFF );
}

void HandleBody( AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total )
{
  String script = ( char* ) data;
  int i = script.indexOf( ';' );
  LuaScript = script.substring( 0, i ); // Filter out garbage data that breaks the script
  ScriptActive = true;
}

void setup()
{
	Serial.begin( 115200 );
	delay( 10 );
	Serial.println();
	Serial.print( "Connecting to " );
	Serial.println( ssid );
	WiFi.begin( ssid, password );
	MDNS.begin( "colorselector" );
	
	if ( !SPIFFS.begin( true ) ) return;

	while ( WiFi.status() != WL_CONNECTED )
	{
		delay( 500 );
		Serial.print( "." );
	}

	FastLED.addLeds<CHIPSET, DATA_PIN, RGB_ORDER>( leds, NUM_LEDS );
	FastLED.clear( true );
	FastLED.setBrightness( 50 );
	FastLED.setCorrection( TypicalPixelString );

	Serial.println();
	Serial.println( "WiFi connected." );
	Serial.print( "IP address: " );
	Serial.println( WiFi.localIP() );

  lua.Lua_register( "SetColor", ( const lua_CFunction ) &SetColor );
  lua.Lua_register( "SetSolidColor", ( const lua_CFunction ) &SetSolidColor );
  lua.Lua_register( "PushColors", ( const lua_CFunction ) &PushColors );
  lua.Lua_register( "delay", ( const lua_CFunction ) &LuaDelay );
  lua.Lua_register( "millis", ( const lua_CFunction ) &LuaMillis );
  lua.Lua_register( "HeatColor", ( const lua_CFunction ) &LuaHeatColor );
  lua.Lua_register( "FadeToBlackBy", ( const lua_CFunction ) &FadeToBlackBy );
  lua.Lua_register( "SetBrightness", ( const lua_CFunction ) &SetBrightness );
  lua.Lua_register( "SetColorHSV", ( const lua_CFunction ) &SetColorHSV );

	server.on( "/", HTTP_GET, []( AsyncWebServerRequest *request ) {
		request->send( SPIFFS, "/index.html", String(), false );
	} );

	server.on( "/state", HTTP_POST, []( AsyncWebServerRequest *request ) {
    ScriptActive = false;
		if ( request->hasParam( "color" ) )
		{
			int color = request->getParam( "color" )->value().toInt();
			for ( int i = 0; i < NUM_LEDS; i++ )
			{
				leds[i] = color;
			}
			FastLED.show();
		}
		else
		{
			size_t numParams = request->params();
			for ( int i = 0; i < numParams; i++ )
			{
				AsyncWebParameter *param = request->getParam( i );
				int color = param->value().toInt();
				String name = param->name();
				if ( name.startsWith( "led" ) )
				{
					name.remove( 0, 3 );
					int ledNum = name.toInt();
					leds[ledNum] = color;
				}
			}
			FastLED.show();
		}
		request->send( 200, "text/plain", "OK" );
	} );

  server.on( "/anim", HTTP_POST, []( AsyncWebServerRequest *request ) {
    request->send( 200 );
  }, NULL, HandleBody );

	server.on( "/settings", HTTP_POST, []( AsyncWebServerRequest * request ) {
		if ( request->hasParam( "brightness" ) )
		{
			int brightness = request->getParam( "brightness" )->value().toInt();
			FastLED.setBrightness( brightness );
		}
    else if ( request->hasParam( "debug" ) )
    {
      bool debug = request->getParam( "debug" )->value().toInt();
      Debug = debug;
    }
		request->send( 200, "text/plain", "OK" );
	} );

	server.on( "/input.js", HTTP_GET, []( AsyncWebServerRequest *request ) {
		request->send( SPIFFS, "/input.js", "text/javascript" );
	} );

	server.on( "/style.css", HTTP_GET, []( AsyncWebServerRequest *request ) {
		request->send( SPIFFS, "/style.css", "text/css" );
	} );

	server.begin();
}

void loop()
{
  if ( ScriptActive )
  {
    if ( Debug )
      Serial.println( lua.Lua_dostring( &LuaScript ) );
    else
      lua.Lua_dostring( &LuaScript );
  }
}
