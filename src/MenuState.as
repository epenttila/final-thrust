package
{
	import org.flixel.*;
	
	public class MenuState extends FlxState
	{
		override public function create():void
		{
			var title:FlxText = new FlxText(0, 16, FlxG.width, "Final Thrust");
			title.setFormat(null, 16, 0xffffff, "center");
			add(title);
			
			var instructions:FlxText = new FlxText(0, FlxG.height - 32, FlxG.width, "Click to begin");
			instructions.setFormat(null, 8, 0xffffff, "center");
			add(instructions);
		}
		
		override public function update():void
		{
			if (FlxG.mouse.justPressed())
				FlxG.state = new PlayState(1);
		}
	}
}
