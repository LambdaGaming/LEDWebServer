// Simple .NET 6 console program to show how the led_web Arduino program can be interacted with outside of a browser

using System.Drawing;

HttpClient client = new();
string input;
bool namedcolor = false;

Console.WriteLine( "Enter a hex number or color name:" );
while ( ( input = Console.ReadLine() ) != null && input != "exit" )
{
	int color = 0;
	int realcolor = Color.FromName( input ).ToArgb();

	if ( realcolor != 0 )
	{
		Console.WriteLine( "Color name detected." );
		color = realcolor;
		namedcolor = true;
	}

	try
	{
		Console.WriteLine( "Attempting to parse and apply color..." );

		if ( !namedcolor )
		{
			if ( !input.StartsWith( "0x" ) )
				input = "0x" + input;

			color = Convert.ToInt32( input, 16 );
		}

		await client.GetAsync( "http://colorselector.local/state?color=" + color );
		Console.WriteLine( "Successfully changed the color to " + input + "\n" );
	}
	catch ( Exception e )
	{
		Console.WriteLine( "\nAn error occured while attempting to change the color:\n" + e.Message + "\n" );
	}

	namedcolor = false;
	Console.WriteLine( "Enter a hex number or color name:" );
}
