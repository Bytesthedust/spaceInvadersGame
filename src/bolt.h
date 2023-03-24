#pragma once

#include <ncurses.h>
#include "drawable.hpp"


class Bolt: public Drawable
{
	public:
		Bolt(int y, int x)
		{
			this->y = y;
			this->x = x;
			this->icon = '*';
		}

		void setY(int newY)
		{
			y = newY;
		}

		void setX(int newX)
		{
			x = newX;
		}

			
};
