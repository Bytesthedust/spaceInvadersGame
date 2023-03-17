//class to create player character and additional characters

class Drawable
{
	public:
		Drawable()
		{
			y=x=0;
			icon = ' ';
		}

		//will display a character given the coordinates
		Drawable(int y, int x, chtype ch)
		{
			this->y = y;
			this->x = x;
			this->icon = ch;
		}

		int getX() //return x coordinate
		{
			return x;
		}

		int getY() //return y coordinate
		{
			return y;
		}

		chtype getIcon() //return icon character
		{
			return icon;
		}

	protected:
		int y, x;
		chtype icon;

};
