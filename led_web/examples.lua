--Random color every 0.5 seconds
local r = math.random( 0, 255 )
local g = math.random( 0, 255 )
local b = math.random( 0, 255 )
local color = ( ( r & 0xFF ) << 16 ) | ( ( g & 0xFF ) << 8 ) | ( b & 0xFF )
SetSolidColor( color )
delay( 500 ); --The semicolon acts as a separator to prevent garbage data from being included in the script

--Alternating orange lights
ColorSwitch = ColorSwitch or false
for i=0,199 do
	if ColorSwitch then
		local even = i % 2 == 0 
		SetColor( i, even and 0xFF5900 or 0 )
	else
		local odd = i % 2 == 1
		SetColor( i, odd and 0xFF5900 or 0 )
	end
end
ColorSwitch = not ColorSwitch
PushColors()
delay( 1000 );

--Snake that goes around the light string
SnakeTable = SnakeTable or { 0xFF, 0xFF0000, 0xFF00, 0xFFFF, 0xFFFF00, 0xFF00FF, 0xFFFFFF, 0xFF5900 }
SnakeIndex = SnakeIndex or 1
Snake = Snake or 0
if Snake > 199 then Snake = 0 end
if SnakeIndex > 8 then SnakeIndex = 1 end
SetColor( Snake, SnakeTable[SnakeIndex] )
if Snake > 10 then
	for i=1,10 do
		SetColor( Snake - i, SnakeTable[SnakeIndex] )
	end
end
if Snake == 0 then
	SetColor( 199, 0 )
	for i=1,10 do
		SetColor( 199 - i, 0 )
	end
	SnakeIndex = SnakeIndex + 1
else
	if Snake > 10 then
		SetColor( Snake - 11, 0 )
	end
end
PushColors()
Snake = Snake + 1
delay( 50 );

--Simple example of how the HeatColor function is used
for i=0,199 do
	local rand = math.random( 0, 255 )
	SetColor( i, HeatColor( rand ) )
end
PushColors()
delay( 30 );

--Simple FadeToBlackBy example
local rand = math.random( 0, 199 )
SetColor( rand, 0xFFFFFF )
PushColors()
FadeToBlackBy( 10 );

--Single color pulsating effect using SetBrightness
local b = math.floor( math.abs( 50 * math.sin( ( millis() * 0.001 ) * 0.5 ) ) )
SetBrightness( b )
SetSolidColor( 0xFF5900 );

--Port of the Cylon FastLED example, minus the fading
Hue = Hue or 0
for i=0,199 do
	Hue = Hue + 1
	SetColorHSV( i, Hue, 255, 255 )
	PushColors()
	delay( 10 )
end
for i=199,0,-1 do
	Hue = Hue + 1
	SetColorHSV( i, Hue, 255, 255 )
	PushColors()
	delay( 10 )
end;

--Simple solid color wheel cycle
Hue = Hue or 0
SetSolidColorHSV( Hue, 255, 255 )
Hue = Hue + 1
delay( 10 );

--Simple FadeToColor example that creates a static 2-color gradient
for i=0,199 do
	FadeToColor( i, 0xFF00, 0xFF, i )
end
PushColors();

--More complex FadeToColor example that constantly fades between blue and green using a sine wave
local sin = beatsin( 30, 50, 255, 0, 0 )
for i=0,199 do
	FadeToColor( i, 0xFF00, 0xFF, sin )
end
PushColors();

--Simplified snake using a sine wave and FadeToBlackBy
local sin = math.floor( math.abs( 199 * math.sin( ( millis() * 0.001 ) * 0.2 ) ) )
SetColor( sin, 0xFF00 )
FadeToBlackBy( 10 )
PushColors();

--Waves of blue and cyan using beatsin, SetBrightness, and FadeToColor
Colors = Colors or {}
if #Colors == 0 then
	for i=0,199 do
		table.insert( Colors, math.random( 0, 255 ) )
	end
end
local sin = beatsin( 5, 50, 255, 0, 0 )
local sin2 = beatsin( 5, 1, 3, 0, 0 )
for i=1,200 do
	SetBrightness( sin )
	FadeToColor( i - 1, 0xFFFF, 0xFF, Colors[i] )
	Colors[i] = Colors[i] + sin2
end
PushColors();
