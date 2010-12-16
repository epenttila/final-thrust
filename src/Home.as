package
{
	import org.flixel.*;
	
	public class Home extends FlxSprite
	{
		[Embed(source = "home.png")]
		private var image:Class;
		
		public function Home(X:Number, Y:Number)
		{
			super(X, Y, image);
			antialiasing = true;
		}
	}

}
