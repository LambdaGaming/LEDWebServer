# LED Web Server
 This project aims to provide an easy way to remotely and dynamically control addressable LED strings with an ESP32, using the FastLED library, the Lua programming language, and a web server. The current list of features can be found below.

- The ESP32 can be connected to the data line of any addressable LED string that the FastLED library supports
- Local domain name: colorselector.local
- Web page interface
  - Standard HTML color picker
  - Hex color input
  - Inputs for applying colors for the entire string or individual LEDs
- Lua interface
  - Lua code is entered from the textbox on the second page, and sent to the server via a POST request
  - There are several Lua functions included that patch through to functions on the Arduino side, and documentation on them can be found [here](doc.md)
  - The script can be run once or as a loop
  - When saving information in the script, global variables need to be used, as local variables are overwritten every loop
  - Scripts currently need to end with a semicolon due to the way they're processed on the backend
  - Some examples can be found [here](led_web/examples.lua)
- WebSocket interface
  - Programs can connect to the server's web socket interface on port 81 to continuously send data
  - Binary data sent to the server needs to be 32 bits each. The first byte is used to specify the index of the LED, and the last 3 bytes are the color data
  - Much more suitable for rapidly updating LEDs compared to the POST requests

# Experiments
There are various [experimental programs](experiments) included that interface with the web server and provide different ways of controlling the lights. While these programs are functional, they aren't complete or designed for regular use, so they are probably more useful as examples or bases to work off of.

# Issues & Pull Requests
 If you would like to contribute to this repository by creating an issue or pull request, please refer to the [contributing guidelines.](https://lambdagaming.github.io/contributing.html)
