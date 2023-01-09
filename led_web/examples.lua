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
