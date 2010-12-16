package
{
	import org.flixel.*;
	
	public class Player extends FlxSprite
	{
		[Embed(source = "ship.png")]
		private var image:Class;
		
		public function Player(X:Number, Y:Number)
		{
			super(X, Y, image);
			antialiasing = true;
		}
		
		override public function update():void
		{
			angularVelocity = 0;
			
			if (FlxG.keys.LEFT) angularVelocity -= 90;
			if (FlxG.keys.RIGHT) angularVelocity += 90;
			
			thrust = 0;
			
			if (FlxG.keys.UP) thrust -= 90;
			
			super.update();
			
			if (x < 0 || x > FlxG.width || y < 0 || y > FlxG.height)
				kill();
		}
	}

}
