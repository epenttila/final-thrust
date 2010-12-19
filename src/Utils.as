package
{
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import org.flixel.*;

	internal class Utils
	{
		public static function getRandomInt(min:Number, max:Number):Number
		{
			return min + Math.floor(FlxU.random() * (max - min + 1));
		}

		public static function getRandomPoint(rect:Rectangle):Point
		{
			var x:Number = getRandomInt(rect.left, rect.right);
			var y:Number = getRandomInt(rect.top, rect.bottom);
		
			return new Point(x, y);
		}
	}
}
