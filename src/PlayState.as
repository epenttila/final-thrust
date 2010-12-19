package
{
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import org.flixel.*;

	/// @todo make all objects derive from same thing and put the radial collision detection there (and getCenter stuff)
	
	public class PlayState extends FlxState
	{
		private static const ZONE_WIDTH:Number = 50;
		private static const ASTEROID_INTERVAL:Number = 0;
		private var _maxAsteroids:Number;
		private var _maxPlanets:Number;
		private var _maxLevel:Number;
		private var _player:Player;
		private var _home:Planet;
		private var _planets:Array;
		private var _asteroids:Array;
		private var _level:Number;
		[Embed(source = "cursor.png")] private var _cursor:Class;
		[Embed(source = "background.png")] private var _background:Class;
		private var _fuelText:FlxText;
		private var _nextState:FlxState;
		
		public function PlayState(level:Number, maxLevel:Number)
		{
			_level = Math.min(level, maxLevel);
			_maxAsteroids = _level + 1;
			_maxPlanets = _level + 1;
			_maxLevel = maxLevel;
		}

		override public function create():void
		{
			FlxG.mouse.show(_cursor, 8, 8);
			initWorld(_level / _maxLevel);
			super.update();
		}
		
		override public function update():void
		{
			if (FlxG.keys.ESCAPE)
				FlxG.state = new MenuState(_level);
				
			if (_nextState)
			{
				if (FlxG.mouse.justPressed())
					FlxG.state = _nextState;
			}
			else
			{
				applyGravity(_player, 100);
				
				for each (var a:Asteroid in _asteroids)
					applyGravity(a, 10);

				_fuelText.text = Math.round(_player.fuel).toString() + "%";
				
				super.update();

				if (isPlayerCollidingWithWorld())
					_player.explode();

				if (_home.isInside(_player.getCenter(), 0))
					nextLevel();
				else if (_player.dead)
					restartLevel();
			}
		}
		
		private function applyGravity(object:FlxObject, coeff:Number):void
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
					const g:Point = new Point(coeff / dd * r.x, coeff / dd * r.y);
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
			
			add(new FlxSprite(0, 0, _background));
			
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
			
			createPlanets();
						
			_asteroids = new Array();
			
			for (var i:* = 0; i < _maxAsteroids; ++i)
				createAsteroid();

			var text:FlxText = new FlxText(0, 0, 100, "Level " + (_level + 1).toString());
			add(text);
			
			_fuelText = new FlxText(0, 0, FlxG.width);
			_fuelText.setFormat(null, 8, 0xffffff, "right");
			add(_fuelText);
		}
		
		private function isPlayerCollidingWithWorld():Boolean
		{
			const point:FlxPoint = new FlxPoint(_player.x + _player.origin.x, _player.y + _player.origin.y);
			
			for each (var p:Planet in _planets)
			{
				if (p.isInside(point, 0))
					return true;
			}
			
			for each (var a:Asteroid in _asteroids)
			{
				if (a.isInside(point, _player.radius))
					return true;
			}

			return false;
		}
		
		private function createAsteroid():void
		{
			const margin:Number = 10;
			const side:Number = Utils.getRandomInt(0, 3);
			const rect:Rectangle = new Rectangle(ZONE_WIDTH, 0, FlxG.width - ZONE_WIDTH, FlxG.height);
			const point:Point = Utils.getRandomPoint(rect);
			var x:Number = point.x;
			var y:Number = point.y;
			
			/*switch (side)
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
			}*/
			
			var asteroid:Asteroid = new Asteroid(x, y);
			_asteroids.push(asteroid);
			add(asteroid);
		}
		
		private function createPlanets():void
		{
			const GRID_SIZE:Number = Math.ceil(Math.sqrt(_maxLevel));
			const GRID_WIDTH:Number = FlxG.width - ZONE_WIDTH - ZONE_WIDTH;
			const GRID_HEIGHT:Number = FlxG.height;
			const CELL_WIDTH:Number = GRID_WIDTH / GRID_SIZE;
			const CELL_HEIGHT:Number = GRID_HEIGHT / GRID_SIZE;
			
			_planets = new Array();
			var occupied:Array = new Array(GRID_SIZE * GRID_SIZE);
			
			for (var i:Number = 0; i < _maxPlanets; )
			{
				const randomX:Number = Utils.getRandomInt(0, GRID_SIZE - 1);
				const randomY:Number = Utils.getRandomInt(0, GRID_SIZE - 1);
				const index:Number = randomX + GRID_SIZE * randomY;
				
				if (occupied[index])
					continue;
				
				occupied[index] = true;
				
				var planet:Planet = new Planet(Utils.getRandomInt(1, Planet.PLANET_TYPES - 1));
				planet.x = ZONE_WIDTH + ((randomX + 0.5) / GRID_SIZE) * GRID_WIDTH - planet.width / 2;
				planet.y = ((randomY + 0.5) / GRID_SIZE) * GRID_HEIGHT - planet.height / 2;
				add(planet);
				_planets.push(planet);
				
				++i;
			}
			
			for each (var p:Planet in _planets)
				add(p);
		}
		
		private function nextLevel():void
		{
			showEndText("You survived!", "Click to continue");
			
			_nextState = new PlayState(_level + 1, _maxLevel);
		}
		
		private function restartLevel():void
		{
			showEndText("You are dead", "Click to try again");
			
			_nextState = new PlayState(_level, _maxLevel);
		}
		
		private function showEndText(string:String, subString:String):void
		{
			const margin:Number = 0;
			const size:Number = 16;
			const subSize:Number = 8;
			
			var text:FlxText = new FlxText(0, 0, FlxG.width, string);
			text.size = size;
			text.alignment = "center";
			text.shadow = 0xff000000;
			add(text);
			
			var subText:FlxText = new FlxText(0, 0, FlxG.width, subString);
			subText.size = subSize;
			subText.alignment = "center";
			subText.shadow = 0xff000000;
			add(subText);

			const totalHeight:Number = text.height + margin + subText.height;
			text.y = (FlxG.height - totalHeight) / 2;
			subText.y = text.y + text.height + margin;
		}
	}
}
