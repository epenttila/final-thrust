package
{
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import org.flixel.*;
	
	public class PlayState extends FlxState
	{
		private static const ZONE_WIDTH:Number = 50;
		private static const MAX_ASTEROIDS:Number = 10;
		private static const ASTEROID_INTERVAL:Number = 1;
		private var _player:Player;
		private var _home:Planet;
		private var _planets:Array;
		private var _asteroids:Array;
		private var _asteroidTimer:Number;
		private var _level:Number;
		[Embed(source = "cursor.png")] private var _cursor:Class;
		
		public function PlayState(level:Number)
		{
			_level = level;
		}

		override public function create():void
		{
			FlxG.mouse.show(_cursor, 8, 8);
			
			initWorld(Math.min(_level / 256, 1));
		}
		
		override public function update():void
		{
			applyGravity(_player);
			
			_asteroidTimer -= FlxG.elapsed;
			
			if (_asteroidTimer < 0 && _asteroids.length < MAX_ASTEROIDS)
				createAsteroid();
			
			for each (var a:Asteroid in _asteroids)
				applyGravity(a);
			
			super.update();

			if (isCollidingWithPlanets(_player))
				_player.kill();

			if (FlxU.overlap(_player, _home) || _player.dead)
			{
				FlxG.state = new PlayState(_level + 1);
			}
		}
		
		private function applyGravity(object:FlxObject):void
		{
			var gravity:Point = new Point();
			
			for each (var planet:Planet in _planets)
			{
				const playerPos:FlxPoint = new FlxPoint(object.x + object.origin.x, object.y + object.origin.y);
				const planetPos:FlxPoint = new FlxPoint(planet.x + planet.origin.x, planet.y + planet.origin.y);
				const r:FlxPoint = new FlxPoint(playerPos.x - planetPos.x, playerPos.y - planetPos.y);
				const dd:Number = (r.x * r.x) + (r.y * r.y);
				
				if (dd > 1)
				{
					const mass:Number = 100;
					const g:Point = new Point(mass / dd * r.x, mass / dd * r.y);
					gravity.x += g.x;
					gravity.y += g.y;
				}
			}

			object.acceleration.x = -gravity.x;
			object.acceleration.y = -gravity.y;
		}
		
		private function initWorld(seed:Number):void
		{
			FlxU.seed = seed;
			
			var text:FlxText = new FlxText(0, 0, 100, "Level " + _level.toString());
			add(text);
			
			_player = new Player();
			_player.x = 16;
			_player.y = (FlxG.height - _player.height) / 2;
			add(_player);

			_home = new Planet(0);
			const homeRect:Rectangle = new Rectangle(FlxG.width - ZONE_WIDTH, 0, ZONE_WIDTH - _home.width, FlxG.height - _home.height);
			const homePoint:Point = Utils.getRandomPoint(homeRect);
			_home.x = homePoint.x;
			_home.y = homePoint.y;
			add(_home);
			
			const planetRect:Rectangle = new Rectangle(ZONE_WIDTH, 0, FlxG.width - ZONE_WIDTH - ZONE_WIDTH, FlxG.height);
			_planets = new Array();
			
			for (var i:Number = 0; i < 10; ++i)
			{
				var point:Point = Utils.getRandomPoint(planetRect);
				var planet:Planet = new Planet(Utils.getRandomInt(1, 3));
				planet.x = point.x;
				planet.y = point.y;
				add(planet);
				_planets.push(planet);
			}
			
			for each (var p:Planet in _planets)
				add(p);
				
			_asteroidTimer = 0;
			_asteroids = new Array();
		}
		
		private function isCollidingWithPlanets(object:FlxObject):Boolean
		{
			const point:FlxPoint = new FlxPoint(object.x + object.origin.x, object.y + object.origin.y);
			
			for each (var p:Planet in _planets)
			{
				if (p.isInside(point))
					return true;
			}
			
			return false;
		}
		
		private function createAsteroid():void
		{
			const margin:Number = 10;
			const side:Number = Utils.getRandomInt(0, 3);
			var x:Number = FlxU.random() * FlxG.width;
			var y:Number = FlxU.random() * FlxG.height;
			
			switch (side)
			{
			case 0: // north
				y = -margin;
				break;
			case 1: // east
				x = FlxG.width + margin;
				break;
			case 2: // south
				y = FlxG.height + margin;
				break;
			case 3: // west
				x = -margin;
				break;
			}
			
			var asteroid:Asteroid = new Asteroid();
			asteroid.x = x;
			asteroid.y = y;
			_asteroids.push(asteroid);
			add(asteroid);
			_asteroidTimer = ASTEROID_INTERVAL;
		}
	}
}
