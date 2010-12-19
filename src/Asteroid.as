package
{
	import org.flixel.*;
	
	public class Asteroid extends RoundObject
	{
		[Embed(source = "asteroid.png")] private const _image:Class;
		
		public function Asteroid(X:Number, Y:Number)
		{
			super(0, X, Y, _image);
			
			const angle:Number = FlxU.random() * 360;
			const speed:Number = FlxU.random() * 10;
			
			velocity.x = Math.sin(angle) * speed;
			velocity.y = Math.cos(angle) * speed;
			angularVelocity = (FlxU.random() - 0.5) * 90;
			
			antialiasing = true;
		}
	}
}
