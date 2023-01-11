# SetSolidColor( color )
- Sets a single color for the whole string
- `color` argument must be a number

# SetColor( index, color )
- Sets a color for a single LED of the specified index
- `index` argument must be a number and must be within the range of the led array in your Arduino sketch
- `color` argument must be a number
- Will throw a Lua error if the index is out of bounds

# PushColors()
- Used to apply colors changed with the `SetColor()` function
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
- 0 is also valid but returns black
