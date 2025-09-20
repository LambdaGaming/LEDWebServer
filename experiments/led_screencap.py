# Python 3.8+ program that reads the color of random pixels on the user's screen and applies them to the LED string every 10 seconds.
# Works best when image on screen is static or very slow moving, with little to no rapid color changes.

from PIL import ImageGrab
import requests
import random
import time

MODIFIER = 1 # Increase or decrease area of the screen without needing a specific number of pixels
WIDTH = 1920 * MODIFIER
HEIGHT = 1080 * MODIFIER
NUM_LEDS = 200

def GetPixels():
	params = ""
	then = time.perf_counter()
	for i in range( NUM_LEDS - 1 ):
		try:
			color = pixel[random.randrange( 0, round( WIDTH ) ), random.randrange( 0, round( HEIGHT ) )]
		except:
			print( "Resolution changed. Aborting." )
			return
		finalcolor = "{:02x}{:02x}{:02x}".format( color[0], color[1], color[2] )
		finalcolor = int( finalcolor, 16 )
		params += "led{0}={1}&".format( i, finalcolor )
	requests.post( "http://colorselector.local/state?" + params )
	now = time.perf_counter()
	print( "Applied colors in {:.3f} seconds.".format( now - then ) )

while True:
	pixel = ImageGrab.grab().load()
	GetPixels()
	time.sleep( 10 )
