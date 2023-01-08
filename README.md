This repository contains four projects that are all tied together, with the main one being led_web. 

# LED Web Server (led_web)
This is the main project that started out with some experiments on a cheap fairy light string. More projects formed and this one grew as I discovered new ways to control the lights. The project consists of a web server made for the ESP32, which is connected to the data cable of the aforementioned light string to act as a driver through the FastLED Arduino library. The current list of features for the server can be found below. My end goal with this project is to allow users to input custom animations dynamically, rather than having to compile it with the main Arduino code.
- Local domain name: colorselector.local
- The web page allows you to input a custom color that will apply to the LED string. There are multiple types of color inputs: 
  - Standard color picker
  - Hex color input
  - Buttons for setting a random color for the entire string and each individual LED
- Specific LEDs within the string can also be set by entering numbers or a range in the textbox on the first page
  - Example 1: `0:199:2` will set every even-numbered LED
  - Example 2: `0,1,2,3,4` will set the first five LEDs
  - Example 3: `0:4` will do the same as example 2
- Animations are also supported through Lua (WIP and not fully implemented yet)
  - Lua code is entered from the textbox on the second page, and sent to the server via a POST request
  - Custom functions include: `SetSolidColor( color )`, `SetColor( index, color )`, `PushColors()`, and `delay( ms )`
  - The script is constantly being executed in the Arduino loop function, so global variables need to be used in order to save information outside of a single loop
  - Scripts currently need to end with a semicolon due to the way they come through on the backend
  - Some examples can be found [here](led_web/examples.lua)

# Color Text Input (led_web_app)
This is a small .NET 6 app that was made as a simple example of how a program could interact with the server. It allows you to type a hex color or a color name that's in the .NET predefined color list. It will automatically convert the input color to an integer and send it to the server.

# Screen Color Capture (led_web_screen)
This is a Python program that reads the color of random pixels on the user's screen and applies them to the LED string every 10 seconds. Having the LED string setup in a dark room while playing games with unique color pallets provides a surprisingly atmospheric experience.

# Color Voice Input (led_speech)
This is an Android app written in Kotlin that functions similarly to the .NET app but takes a voice input instead of text. There is a predefined list of colors based off the one found in .NET that the user can say.
