package
{
	import flash.display.Graphics;
	import org.flixel.*;
	
	public class Planet extends RoundObject
	{
		public static const PLANET_TYPES:Number = 5;
		[Embed(source = "home.png")] private var _earthImage:Class;
		[Embed(source = "jupiter.png")] private var _jupiterImage:Class;
		[Embed(source = "uranus.png")] private var _uranusImage:Class;
		[Embed(source = "saturn.png")] private var _saturnImage:Class;
		[Embed(source = "mars.png")] private var _marsImage:Class;
		
		public function Planet(size:Number)
		{
			var image:Class;
			var radius:Number;
			
			switch (size)
			{
			case 0: image = _earthImage; radius = 8; break;
			case 1: image = _jupiterImage; radius = 15; break;
			case 2: image = _uranusImage; radius = 12; break;
			case 3: image = _saturnImage; radius = 10; break;
			case 4: image = _marsImage; radius = 8; break;
			}
			
			super(radius, 0, 0, image);
			
			//_radius = Math.min(width, height) / 2;
		}
	}
}
