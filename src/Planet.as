package
{
	import flash.display.Graphics;
	import org.flixel.*;
	
	public class Planet extends FlxSprite
	{
		[Embed(source = "home.png")] private var _earthImage:Class;
		[Embed(source = "bigplanet.png")] private var _jupiterImage:Class;
		[Embed(source = "uranus.png")] private var _uranusImage:Class;
		[Embed(source = "saturn.png")] private var _saturnImage:Class;
		private var _radius:Number;
		
		public function Planet(size:Number)
		{
			var image:Class;
			
			switch (size)
			{
			case 0: image = _earthImage; _radius = 8; break;
			case 1: image = _jupiterImage; _radius = 15; break;
			case 2: image = _uranusImage; _radius = 12; break;
			case 3: image = _saturnImage; _radius = 10; break;
			}
			
			super(0, 0, image);
			
			//_radius = Math.min(width, height) / 2;
		}
		
		public function isInside(point:FlxPoint):Boolean
		{
			const dx:Number = point.x - (x + origin.x);
			const dy:Number = point.y - (y + origin.y);
			const dd:Number = dx * dx + dy * dy;
			
			return (dd < _radius * _radius);
		}
	}
}
