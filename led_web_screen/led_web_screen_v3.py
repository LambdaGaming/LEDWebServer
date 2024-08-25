from PIL import ImageGrab
from websockets.sync.client import connect
import random
import time

MODIFIER = 0.5
WIDTH = 1920
HEIGHT = 1080
NUM_LEDS = 200
RES_OVERRIDE = None # Override box where pixels are read from. Format: ( left_x, top_y, right_x, bottom_y ) or None; Example: ( 0, 1080, 2160, 1920 )

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

with connect( "ws://colorselector.local:81" ) as ws:
	try:
		while True:
			image = ImageGrab.grab( bbox = RES_OVERRIDE )
			colors = image.getcolors()
			print(colors)
			pixel = image.load()
			data = []
			count = 0
			for p in CalcRandomPixels():
				color = pixel[p[0], p[1]]
				data.append( count )
				data.extend( ( color[0], color[1], color[2] ) )
				count += 1
			ws.send( bytearray( data ) )
			#time.sleep( 0.016 ) # ~60 FPS
			time.sleep( 1 )
	except KeyboardInterrupt:
		ws.close()
		print( "Connection closed." )
