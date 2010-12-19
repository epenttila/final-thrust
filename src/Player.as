package
{
	import flash.geom.Point;
	import flash.media.Sound;
	import org.flixel.*;
	
	public class Player extends FlxGroup
	{
		[Embed(source = "ship.png")] private var _image:Class;
		[Embed(source = "exhaust.png")] private var _exhaustImage:Class;
		[Embed(source = "explosion.png")] private var _explosionImage:Class;
		private var _ship:FlxSprite;
		private var _exhaust:FlxSprite;
		private var _explosion:FlxSprite;
		private var _fuel:Number;
		private var _exploding:Boolean;
		private var _thrustSound:FlxSound;
		
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
			
			_explosion = new FlxSprite();
			_explosion.loadGraphic(_explosionImage, true);
			_explosion.addAnimation("explode", [0, 1, 2, 3, 4, 5, 6], 7, false);
			_explosion.x = (_ship.width - _explosion.width) / 2;
			_explosion.y = (_ship.height - _explosion.height) / 2;
			_explosion.visible = false;
			add(_explosion);
			
			origin = _ship.origin;
			
			_fuel = 100;
			_exploding = false;
		}
		
		override public function update():void
		{
			angle = FlxU.getAngle(FlxG.mouse.x - (x + (width / 2)), FlxG.mouse.y - (y + (height / 2)));
			
			thrust = 0;
			_exhaust.visible = false;
			
			if (!_exploding)
			{
				if (FlxG.mouse.pressed() && _fuel > 0)
				{
					thrust -= 90;
					_exhaust.visible = true;
					_fuel -= 1;
					
					if (!_thrustSound)
						_thrustSound = FlxG.play(thrustSound, 0.1, true);
				}
				else
				{
					if (_thrustSound)
					{
						_thrustSound.stop();
						_thrustSound = null;
					}
				}
			}
			else
			{
				velocity.x = 0;
				velocity.y = 0;
			}
			
			super.update();
			
			if (x + _ship.width < 0 || x > FlxG.width || y + _ship.height < 0 || y > FlxG.height)
				kill();
				
			if (_exploding && _explosion.finished)
				kill();
				
			_ship.play("idle");
		}
		
		override public function render():void
		{
			const tmp:Number = angle;
			angle = 0;
			_exhaust.angle = tmp;
			super.render();
			angle = tmp;
		}
		
		public function getCenter():FlxPoint
		{
			return new FlxPoint(x + origin.x, y + origin.y);
		}
		
		public function get fuel():Number
		{
			return _fuel;
		}
		
		public function explode():void
		{
			if (_exploding)
				return;
				
			_explosion.visible = true;
			_explosion.play("explode");
			_exploding = true;
			_ship.visible = false;
			
			FlxG.play(explosionSound, 0.1);
		}
		
		public function get radius():Number
		{
			return _ship.width / 2;
		}
	}
}
