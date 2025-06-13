# Sets LEDs to white at a brightness that's based on the decibel level of the default input device

import pyaudio
import numpy as np
import time
from websockets.sync.client import connect

CHUNK = 4096
RATE = 44100
NUM_LEDS = 200
PEAK_DB = 100
p = pyaudio.PyAudio()
stream = p.open( format=pyaudio.paInt16, channels=1, rate=RATE, input=True, frames_per_buffer=CHUNK, input_device_index=p.get_default_input_device_info()["index"] )

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
			brightness = np.clip( round( 255 * ( db / PEAK_DB ) ), 0, 255 )
			for i in range( NUM_LEDS ):
				data.append( i )
				data.extend( ( brightness, brightness, brightness ) )
			ws.send( bytearray( data ) )
			time.sleep( 0.016 )
	except KeyboardInterrupt:
		ws.close()
		print( "Connection closed." )
