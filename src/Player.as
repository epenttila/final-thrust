package
{
	import flash.geom.Point;
	import org.flixel.*;
	
	public class Player extends FlxGroup
	{
		[Embed(source = "ship.png")] private var _image:Class;
		[Embed(source = "exhaust.png")] private var _exhaustImage:Class;
		private var _ship:FlxSprite;
		private var _exhaust:FlxSprite;
		
		public function Player()
		{
			super();
			
			_ship = new FlxSprite();
			_ship.loadGraphic(_image, true, false, 8, 8);
			_ship.addAnimation("idle", [0, 1], 2);
			add(_ship);
			
			_exhaust = new FlxSprite(0, 0, _exhaustImage);
			_exhaust.visible = true;
			_exhaust.x = -_exhaust.width;
			_exhaust.origin.x = _exhaust.width + _ship.width / 2;
			_exhaust.antialiasing = true;
			add(_exhaust);
		}
		
		override public function update():void
		{
			angularVelocity = 0;
			
			if (FlxG.keys.LEFT) angularVelocity -= 90;
			if (FlxG.keys.RIGHT) angularVelocity += 90;
			
			angle = FlxU.getAngle(FlxG.mouse.x - (x + (width / 2)), FlxG.mouse.y - (y + (height / 2)));
			
			thrust = 0;
			
			if (FlxG.mouse.pressed()) thrust -= 90;
			
			super.update();
			
			if (x + width < 0 || x > FlxG.width || y + height < 0 || y > FlxG.height)
				kill();
				
			_ship.play("idle");
			
			_exhaust.visible = false;
			//_exhaustN.visible = false;

			if (thrust < 0)
			{
				_exhaust.visible = true;
				//_exhaustN.visible = true;
			}
		}
		
		override public function render():void
		{
			const tmp:Number = angle;
			angle = 0;
			_exhaust.angle = tmp;
			super.render();
			angle = tmp;
		}
	}
}
