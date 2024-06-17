# Version 2 of the Screen Color Capture program. Uses a websocket to constantly update the lights at a higher frequency than what the original program can do.

from PIL import ImageGrab
from websockets.sync.client import connect
import random
import time

MODIFIER = 0.5 # Increase or decrease area of the screen without needing a specific number of pixels
WIDTH = 1920
HEIGHT = 1080
NUM_LEDS = 200
CENTER_W = WIDTH * 0.5
CENTER_H = HEIGHT * 0.5

# Reads a square of pixels in the center of the screen, the size of which depends on the amount of LEDs
# There's definitely a better way of doing this but I'm bad at math
def CalcSquare():
	pixels = []
	quarter = int( NUM_LEDS * 0.25 )
	corners = [
		[CENTER_W - quarter, CENTER_H + quarter], # Upper right
		[CENTER_W + quarter, CENTER_H - quarter] # Lower left
	]
	for i in range( quarter ):
		pixels.append( [corners[0][0] + i, corners[0][1]] )
	for i in range( quarter ):
		pixels.append( [corners[0][0], corners[0][1] - i] )
	for i in range( quarter ):
		pixels.append( [corners[1][0] - i, corners[1][1]] )
	for i in range( quarter ):
		pixels.append( [corners[1][0], corners[1][1] + i] )
	return pixels

def CalcRandomPixels():
	pixels = []
	for i in range( NUM_LEDS ):
		if MODIFIER < 1:
			halfX = round( WIDTH * MODIFIER )
			halfY = round( HEIGHT * MODIFIER )
			randX = random.randrange( int( halfX * 0.5 ), int( WIDTH - ( halfX * 0.5 ) ) )
			randY = random.randrange( int( halfY * 0.5 ), int( HEIGHT - ( halfY * 0.5 ) ) )
		else:
			randX = random.randrange( 0, WIDTH )
			randY = random.randrange( 0, HEIGHT )
		pixels.append( [randX, randY] )
	return pixels

mode = int( input( "Enter mode: " ) )
match mode:
	case 0:
		PixelList = CalcSquare()
	case 1:
		PixelList = CalcRandomPixels()
	case _:
		print( "Invalid mode set" )
		exit()

with connect( "ws://colorselector.local:81" ) as ws:
	try:
		while True:
			pixel = ImageGrab.grab().load()
			data = []
			count = 0
			for p in PixelList:
				color = pixel[p[0], p[1]]
				data.append( count )
				data.extend( ( color[0], color[1], color[2] ) )
				count += 1
			ws.send( bytearray( data ) )
			time.sleep( 0.01 )
	except KeyboardInterrupt:
		ws.close()
		print( "Connection closed." )
