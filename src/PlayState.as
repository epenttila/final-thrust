package
{
	import org.flixel.*;
	
	public class PlayState extends FlxState
	{
		private var _player:Player;
		private var _home:Home;
		
		override public function create():void
		{
			_player = new Player(0, FlxG.height / 2);
			add(_player);
			
			_home = new Home(FlxG.width - 16, FlxG.height / 2);
			add(_home);
		}
		
		override public function update():void
		{
			super.update();
			
			if (FlxU.overlap(_player, _home) || _player.dead)
				FlxG.state = new PlayState();
		}
	}
}
