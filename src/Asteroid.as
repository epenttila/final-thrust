package
{
	import org.flixel.FlxSprite;
	
	public class Asteroid extends FlxSprite
	{
		[Embed(source = "asteroid.png")] private const _image:Class;
		
		public function Asteroid()
		{
			super(0, 0, _image);
		}
	}
}
