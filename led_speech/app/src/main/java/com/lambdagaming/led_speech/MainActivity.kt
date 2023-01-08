package com.lambdagaming.led_speech

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.speech.RecognitionListener
import android.speech.RecognizerIntent
import android.speech.SpeechRecognizer
import android.widget.TextView
import android.widget.ToggleButton
import com.android.volley.Request
import com.android.volley.toolbox.StringRequest
import com.android.volley.toolbox.Volley
import kotlinx.serialization.*
import kotlinx.serialization.json.*
import java.io.BufferedReader

@Serializable
data class ColorEntry( val name: String, val color: Int )

class MainActivity : AppCompatActivity(), RecognitionListener {
	override fun onCreate(savedInstanceState: Bundle? ) {
		super.onCreate( savedInstanceState )
		setContentView( R.layout.activity_main )

		val button = findViewById<ToggleButton>( R.id.toggleButton )
		val speech = SpeechRecognizer.createSpeechRecognizer( this )
		if ( SpeechRecognizer.isRecognitionAvailable( this ) ) {
			speech.setRecognitionListener( this )
			val recognizerIntent = Intent( RecognizerIntent.ACTION_RECOGNIZE_SPEECH )
			recognizerIntent.putExtra( RecognizerIntent.EXTRA_LANGUAGE_PREFERENCE, "US-en" )
			recognizerIntent.putExtra( RecognizerIntent.EXTRA_LANGUAGE_MODEL, RecognizerIntent.LANGUAGE_MODEL_FREE_FORM )
			recognizerIntent.putExtra( RecognizerIntent.EXTRA_MAX_RESULTS, 3 )
			button.setOnCheckedChangeListener { _, isChecked ->
				if ( isChecked ) {
					speech.startListening( recognizerIntent )
					button.textOn = "Listening..."
				}
				else {
					speech.stopListening()
					button.textOff = "Off"
				}
			}
		}
	}

	override fun onResults( results: Bundle? ) {
		val text = findViewById<TextView>( R.id.textView )
		val button = findViewById<ToggleButton>( R.id.toggleButton )
		val matches = results!!.getStringArrayList( SpeechRecognizer.RESULTS_RECOGNITION )
		val result = matches?.get( 0 )
		text.text = result
		button.toggle()
		sendColor( result )
	}

	private fun sendColor( color: String? ) {
		val colors = Json.decodeFromString<List<ColorEntry>>( getJSON() )
		colors.forEach { entry ->
			if ( entry.name.lowercase() == color?.lowercase()?.filter { !it.isWhitespace() } ) {
				val queue = Volley.newRequestQueue( this )
				val request = StringRequest( Request.Method.POST, "http://colorselector.local/state?color=${entry.color}", {}, {} )
				queue.add( request )
			}
		}
	}

	private fun getJSON(): String {
		return assets.open( "colors.json" ).bufferedReader().use( BufferedReader::readText )
	}

	override fun onReadyForSpeech(p0: Bundle?) {}
	override fun onBeginningOfSpeech() {}
	override fun onRmsChanged(p0: Float) {}
	override fun onBufferReceived(p0: ByteArray?) {}
	override fun onEndOfSpeech() {}
	override fun onError(p0: Int) {}
	override fun onPartialResults(p0: Bundle?) {}
	override fun onEvent(p0: Int, p1: Bundle?) {}
}
