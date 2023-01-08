# Python 3.8+ program that takes colors from the screen and applies it to the LED string

from PIL import ImageGrab
import requests
import random
import time

WIDTH = 1920
HEIGHT = 1080
NUM_LEDS = 199 # Actually 200 but the IDs start at 0

def GetPixels():
	params = ""
	then = time.perf_counter()
	for i in range( NUM_LEDS ):
		try:
			color = pixel[random.randrange( 0, WIDTH ), random.randrange( 0, HEIGHT )]
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
