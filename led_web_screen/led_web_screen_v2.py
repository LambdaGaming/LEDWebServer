# Version 2 of the LED Web Screen program. Uses a websocket to constantly update the lights at a higher frequency than what the original program can do.

from PIL import ImageGrab
from websockets.sync.client import connect
import random
import time

MODIFIER = 1 # Increase or decrease area of the screen without needing a specific number of pixels
WIDTH = 1920 * MODIFIER
HEIGHT = 1080 * MODIFIER
NUM_LEDS = 200

RandomPixels = []
for i in range( NUM_LEDS ):
	randX = random.randrange( 0, round( WIDTH ) )
	randY = random.randrange( 0, round( HEIGHT ) )
	RandomPixels.append( [randX, randY] )

with connect( "ws://colorselector.local:81" ) as ws:
	try:
		while True:
			pixel = ImageGrab.grab().load()
			data = []
			count = 0
			for p in RandomPixels:
				color = pixel[p[0], p[1]]
				data.append( count )
				data.extend( ( color[0], color[1], color[2] ) )
				count += 1
			ws.send( bytearray( data ) )
			time.sleep( 0.01 )
	except KeyboardInterrupt:
		ws.close()
		print( "Connection closed." )
