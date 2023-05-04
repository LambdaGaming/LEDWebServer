#include <FastLED.h>
#include <LuaWrapper.h>

#define NUM_LEDS 200
CRGB leds[NUM_LEDS];

int DeconstructCRGB( CRGB color )
{
  return ( ( color.r & 0xFF ) << 16 ) | ( ( color.g & 0xFF ) << 8 ) | ( color.b & 0xFF );
}

int SetColor( lua_State *lua_state )
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

int SetSolidColor( lua_State *lua_state )
{
  int color = luaL_checkinteger( lua_state, 1 );
  FastLED.showColor( color );
  return 0;
}

int PushColors( lua_State *lua_state )
{
  FastLED.show();
  return 0;
}

int LuaDelay( lua_State *lua_state )
{
  int time = luaL_checkinteger( lua_state, 1 );
  delay( time );
  return 0;
}

int LuaMillis( lua_State *lua_state )
{
  lua_pushnumber( lua_state, ( lua_Number ) millis() );
  return 1;
}

int LuaHeatColor( lua_State *lua_state )
{
  uint8_t value = ( uint8_t ) luaL_checkinteger( lua_state, 1 );
  lua_pushnumber( lua_state, ( lua_Number ) DeconstructCRGB( HeatColor( value ) ) );
  return 1;
}

int FadeToBlackBy( lua_State *lua_state )
{
  uint8_t value = ( uint8_t ) luaL_checkinteger( lua_state, 1 );
  fadeToBlackBy( leds, NUM_LEDS, value );
  return 0;
}

int SetBrightness( lua_State *lua_state )
{
  uint8_t value = ( uint8_t ) luaL_checkinteger( lua_state, 1 );
  FastLED.setBrightness( value );
  return 0;
}

int SetColorHSV( lua_State *lua_state )
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

int SetSolidColorHSV( lua_State *lua_state )
{
  uint8_t hue = ( uint8_t ) luaL_checkinteger( lua_state, 1 );
  uint8_t sat = ( uint8_t ) luaL_checkinteger( lua_state, 2 );
  uint8_t val = ( uint8_t ) luaL_checkinteger( lua_state, 3 );
  FastLED.showColor( CHSV( hue, sat, val ) );
  return 0;
}

int LuaColorFromPalette( lua_State *lua_state )
{
  luaL_checktype( lua_state, 1, LUA_TTABLE );
  uint8_t index = ( uint8_t ) luaL_checkinteger( lua_state, 2 );
  lua_gettable( lua_state, 1 );
  
  int count = 0;
  CRGBPalette16 colors;
  lua_pushnil( lua_state );
  while ( lua_next( lua_state, 1 ) != 0 )
  {
    colors[count] = ( int ) lua_tonumber( lua_state, -1 );
    lua_pop( lua_state, 1 );
  }
  lua_pushnumber( lua_state, ColorFromPalette( colors, index ) );
  return 1;
}

int FadeToColor( lua_State *lua_state )
{

}
