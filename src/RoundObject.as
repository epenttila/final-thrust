package
{
	import org.flixel.*;
	
	public class RoundObject extends FlxSprite
	{
		private var _radius:Number;
		private var _rr:Number;

		public function RoundObject(radius:Number, X:Number, Y:Number, SimpleGraphic:Class)
		{
			super(X, Y, SimpleGraphic);
			
			_radius = (radius > 0) ? radius : Math.min(width, height) / 2;
			_rr = _radius * _radius;
		}

		public function isInside(point:FlxPoint, radius:Number):Boolean
		{
			const dx:Number = point.x - (x + origin.x);
			const dy:Number = point.y - (y + origin.y);
			const dd:Number = dx * dx + dy * dy;
			
			return (dd <= (_rr + (radius * radius)));
		}
	}
}
