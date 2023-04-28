#include <FastLED.h>
#include <LuaWrapper.h>

#define NUM_LEDS 200
CRGB leds[NUM_LEDS];

int DeconstructCRGB( CRGB color )
{
  return ( ( color.r & 0xFF ) << 16 ) | ( ( color.g & 0xFF ) << 8 ) | ( color.b & 0xFF );
}

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

static int SetSolidColorHSV( lua_State *lua_state )
{
  uint8_t hue = ( uint8_t ) luaL_checkinteger( lua_state, 1 );
  uint8_t sat = ( uint8_t ) luaL_checkinteger( lua_state, 2 );
  uint8_t val = ( uint8_t ) luaL_checkinteger( lua_state, 3 );
  FastLED.showColor( CHSV( hue, sat, val ) );
  return 0;
}
