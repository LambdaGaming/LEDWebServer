This repository contains three projects that are all tied together, with the main one being led_web. Various Arduino sketches that I made to experiment with the FastLED and IRremote libraries are also included in this repo.

# LED Web Server (led_web)
This is the main project that started out with some experiments on a cheap fairy light string. It consists of a web server made for the ESP32, which is connected to the data line of the light string to act as a driver through the FastLED Arduino library. The current list of features for the server can be found below.
- Local domain name: colorselector.local
- The web page allows you to input a custom color that will apply to the LED string. There are multiple types of color inputs: 
  - Standard color picker
  - Hex color input
  - Buttons for setting a random color for the entire string and each individual LED
- Specific LEDs within the string can also be set by entering numbers or a range in the textbox on the first page
  - Example 1: `0:199:2` will set every even-numbered LED
  - Example 2: `0,1,2,3,4` will set the first five LEDs
  - Example 3: `0:4` will do the same as example 2
- Animations are also supported through Lua
  - Lua code is entered from the textbox on the second page, and sent to the server via a POST request
  - There are several Lua functions included that patch through to functions on the Arduino side, and documentation on them can be found [here](doc.md)
  - The script can be run once or as a loop
  - When saving information in the script, global variables need to be used, as local variables are overwritten every loop
  - Scripts currently need to end with a semicolon due to the way they're processed on the backend
  - Some examples can be found [here](led_web/examples.lua)

# Screen Color Capture (led_web_screen)
## V1
Python 3.8+ program that reads the color of random pixels on the user's screen and applies them to the LED string every 10 seconds. Works best when image on screen is static or very slow moving, with little to no rapid color changes.

## V2
Python 3.10+ program that reads the color of pixels on the user's screen through multiple different methods, and applies them to the LED string every 10 milliseconds through a websocket connection. Images on screen that are moving quickly but steadily work well with this version.

# Color Voice Input (led_speech)
Android app written in Kotlin that takes a voice input and applies a color if the input matches a color from a predefined list.
