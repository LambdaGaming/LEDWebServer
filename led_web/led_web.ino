#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "lua_funcs.hpp"

#define DATA_PIN 25
#define CHIPSET WS2812
#define RGB_ORDER RGB

const char *ssid = "";
const char *password = "";
AsyncWebServer server( 80 );
WebSocketsServer webSocket = WebSocketsServer( 81 );
LuaWrapper lua;
String LuaScript = "";
bool ScriptActive = false;

void HandleBody( AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total )
{
  digitalWrite( LED_BUILTIN, HIGH );
  String script = ( char* ) data;
  int i = script.indexOf( ';' );
  LuaScript = script.substring( 0, i ); // Filter out garbage data that breaks the script

  if ( request->hasParam( "single" ) )
    lua.Lua_dostring( &LuaScript );
  else
    ScriptActive = true;

  digitalWrite( LED_BUILTIN, LOW );
}

void WebSocketRequest( uint8_t num, WStype_t type, uint8_t *payload, size_t len )
{
  switch ( type )
  {
    case WStype_CONNECTED:
    {
      ScriptActive = false;
      IPAddress ip = webSocket.remoteIP( num );
      Serial.printf( "Client %u connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3] );
      break;
    }
    case WStype_DISCONNECTED:
    {
      Serial.printf( "Client %u disconnected\n", num );
      break;
    }
    case WStype_BIN:
    {
      if ( len % 4 == 0 )
      {
        for ( int i = 0; i < len; i += 4 )
        {
          uint8_t r = payload[i + 1];
          uint8_t g = payload[i + 2];
          uint8_t b = payload[i + 3];
          leds[payload[i]] = CRGB( r, g, b );
        }
        Serial.printf( "Received %d byte packet\n", len );
        FastLED.show();
      }
      else
      {
        Serial.println( "Received malformed packet!" );
      }
      break;
    }
    default: break;
  }
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
  webSocket.begin();
  webSocket.onEvent( WebSocketRequest );
  pinMode( LED_BUILTIN, OUTPUT );
  
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
  lua.Lua_register( "SetSolidColorHSV", ( const lua_CFunction ) &SetSolidColorHSV );
  lua.Lua_register( "FadeToColor", ( const lua_CFunction ) &FadeToColor );
  lua.Lua_register( "beat", ( const lua_CFunction ) &LuaBeat );
  lua.Lua_register( "beatsin", ( const lua_CFunction ) &LuaBeatSin );

  server.on( "/", HTTP_GET, []( AsyncWebServerRequest *request ) {
    request->send( SPIFFS, "/index.html", String(), false );
  } );

  server.on( "/state", HTTP_POST, []( AsyncWebServerRequest *request ) {
    digitalWrite( LED_BUILTIN, HIGH );
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
        const AsyncWebParameter *param = request->getParam( i );
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
    digitalWrite( LED_BUILTIN, LOW );
  } );

  server.on( "/anim", HTTP_POST, []( AsyncWebServerRequest *request ) {
    request->send( 200 );
  }, NULL, HandleBody );

  server.on( "/settings", HTTP_POST, []( AsyncWebServerRequest * request ) {
    digitalWrite( LED_BUILTIN, HIGH );
    if ( request->hasParam( "brightness" ) )
    {
      int brightness = request->getParam( "brightness" )->value().toInt();
      FastLED.setBrightness( brightness );
    }
    request->send( 200, "text/plain", "OK" );
    digitalWrite( LED_BUILTIN, LOW );
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
    lua.Lua_dostring( &LuaScript );
  else
    webSocket.loop();
}
