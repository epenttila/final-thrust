package
{
	import flash.media.Sound;
	import org.flixel.*;
	
	public class MenuState extends FlxState
	{
		public static const MAX_LEVEL:Number = 81;
		[Embed(source = "menu.png")] private var _background:Class;
		[Embed(source = "button.png")] private var _buttonImage:Class;
		[Embed(source = "buttonhover.png")] private var _buttonHoverImage:Class;
		[Embed(source = "buttonleftoff.png")] private var _buttonLeftOffImage:Class;
		[Embed(source = "buttonlefton.png")] private var _buttonLeftOnImage:Class;
		[Embed(source = "buttonrightoff.png")] private var _buttonRightOffImage:Class;
		[Embed(source = "buttonrighton.png")] private var _buttonRightOnImage:Class;
		[Embed(source = "title.png")] private var _titleImage:Class;
		private var _playButton:FlxButton;
		private var _levelText:FlxText;
		private var _level:Number;
		private var _playTimer:Number;
		private var _playPressed:Boolean;

		public function MenuState(level:Number = 0):void
		{
			_level = level;
			_playTimer = 0.5;
			_playPressed = false;
		}
		
		override public function create():void
		{
			FlxG.playMusic(solitudeMusic, 0.1);
			
			FlxG.mouse.load(null);
			FlxG.mouse.show();
			
			add(new FlxSprite(0, 0, _background));

			//var title:FlxText = new FlxText(0, 16, FlxG.width, "Final Thrust");
			//title.setFormat(null, 24, 0xffffff, "center");
			var title:FlxSprite = new FlxSprite(0, 0, _titleImage);
			title.x = (FlxG.width - title.width) / 2;
			title.y = 16;
			add(title);
			
			var help:FlxText = new FlxText(0, FlxG.height - 16, FlxG.width, "Click to accelerate, ESC to quit");
			help.setFormat(null, 8, 0x808080, "center");
			add(help);

			const buttonImage:FlxSprite = new FlxSprite(0, 0, _buttonImage);
			const buttonHoverImage:FlxSprite = new FlxSprite(0, 0, _buttonHoverImage);
			const buttonWidth:Number = buttonImage.width;
			const buttonHeight:Number = buttonImage.height;
			
			_playButton = new FlxButton((FlxG.width - buttonWidth) / 2, FlxG.height - 60, play);
			_playButton.loadGraphic(buttonImage, buttonHoverImage);
			var playText:FlxText = new FlxText(0, 0, _playButton.width, "PLAY");
			playText.alignment = "center";
			_playButton.loadText(playText);
			add(_playButton);
			
			const buttonMargin:Number = 3;
			
			var leftButton:FlxButton = new FlxButton(_playButton.x, _playButton.y + _playButton.height + buttonMargin, levelDown);
			leftButton.loadGraphic(new FlxSprite(0, 0, _buttonLeftOffImage), new FlxSprite(0, 0, _buttonLeftOnImage));
			add(leftButton);

			var rightButton:FlxButton = new FlxButton(0, 0, levelUp);
			rightButton.loadGraphic(new FlxSprite(0, 0, _buttonRightOffImage), new FlxSprite(0, 0, _buttonRightOnImage));
			rightButton.x = _playButton.x + _playButton.width - rightButton.width;
			rightButton.y = _playButton.y + _playButton.height + buttonMargin;
			add(rightButton);
			
			_levelText = new FlxText(leftButton.x + leftButton.width, leftButton.y, rightButton.x - (leftButton.x + leftButton.width));
			_levelText.alignment = "center";
			add(_levelText);
		}
		
		private function play():void
		{
			FlxG.music.fadeOut(_playTimer);
			_playPressed = true;
		}
		
		public override function update():void
		{
			_levelText.text = "LVL " + (_level + 1).toString();

			if (_playPressed)
				_playTimer -= FlxG.elapsed;
			
			if (_playTimer <= 0)
			{
				FlxG.playMusic(variablesMusic, 0.1);
				FlxG.state = new PlayState(_level, MAX_LEVEL);
			}

			super.update();
		}
		
		private function levelDown():void
		{
			--_level;
			
			if (_level < 0)
				_level = 0;
		}
		
		private function levelUp():void
		{
			++_level;
			
			if (_level >= MAX_LEVEL)
				_level = MAX_LEVEL - 1;
		}
	}
}
