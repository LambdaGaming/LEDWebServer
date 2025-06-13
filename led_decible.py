# Sets LEDs to white at a brightness that's based on the decibel level of the default input device

import pyaudio
import numpy as np
import time
import math
from websockets.sync.client import connect

CHUNK = 1024
NUM_LEDS = 200

p = pyaudio.PyAudio()
for i in range( p.get_device_count() ):
	info = p.get_device_info_by_index( i )
	print( str( i ) + " - " + info["name"] )
DEVICE = int( input( "Enter index of device you want to use: " ) )
RATE = int( p.get_device_info_by_index( DEVICE )["defaultSampleRate"] )
stream = p.open( format=pyaudio.paInt16, channels=1, rate=RATE, input=True, frames_per_buffer=CHUNK, input_device_index=DEVICE )

# Decibel counter modified from https://github.com/itisrmk/Python_decibel-counter/blob/master/Db_Counter.py
def dbCounter():
	data = np.frombuffer( stream.read( CHUNK ), dtype=np.int16 )
	peakSound = np.average( np.abs( data ) ) * 2
	if peakSound > 0:
		decibel = int( 20 * np.log10( peakSound ) )
	else:
		decibel = 0
	print( str( decibel ) + " Db" )
	return decibel

with connect( "ws://colorselector.local:81" ) as ws:
	try:
		while True:
			data = []
			db = dbCounter()
			finalDb = db ** 1.75 * 0.1
			brightness = np.clip( round( finalDb ), 0, 255 )
			for i in range( NUM_LEDS ):
				data.append( i )
				data.extend( ( brightness, brightness, brightness ) )
			ws.send( bytearray( data ) )
			time.sleep( 0.032 )
	except KeyboardInterrupt:
		ws.close()
		print( "Connection closed." )
