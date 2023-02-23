# SetSolidColor( color )
- Sets a single color for the whole string
- `color` argument must be a number

# SetColor( index, color )
- Sets a color for a single LED of the specified index
- `index` argument must be a number and must be within the range of the led array in your Arduino sketch
- `color` argument must be a number
- Will throw a Lua error if the index is out of bounds

# SetSolidColorHSV( hue, saturation, value )
- Sets an HSV value for the whole string
- `hue`, `saturation`, and `value` arguments all must be a number from 0 to 255

# SetColorHSV( index, hue, saturation, value )
- Sets an HSV value on the specified LED
- `index` argument must be a number and must be within the range of the led array in your Arduino sketch
- `hue`, `saturation`, and `value` arguments all must be a number from 0 to 255
- Will throw a Lua error if the index is out of bounds

# PushColors()
- Used to apply colors changed with the `SetColor()` and `SetColorHSV()` functions
- Should only be called once per frame
- Does not need to be used with `SetSolidColor()`

# delay( ms )
- Functions exactly the same as the Arduino function of the same name
- `ms` argument must be a number, in milliseconds

# millis()
- Functions exactly the same as the Arduino function of the same name

# HeatColor( temp )
- Returns a color based on a given temperature value
- `temp` argument must be a number from 1 to 255
- 1 is the coldest (dark red), and 255 is the hottest (white)
- 0 is also valid and returns black

# FadeToBlackBy( brightness )
- Gradually fades LEDs to black by reducing their brightness by the specified amount
- `brightness` argument must be a number from 0 to 255
- 0 doesn't reduce the brightness at all, at 255 reduces the brightness of an LED in a single frame

# SetBrightness( brightness )
- Sets the brightness for the whole string
- Will not take effect until the next `SetSolidColor()` or `PushColors()` call
- `brightness` argument must be a whole number from 0 to 255
- 0 is the darkest (black) and 255 is the brightest
