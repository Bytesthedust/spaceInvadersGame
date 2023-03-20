#pragma once

#include <ncurses.h>
#include "drawable.hpp"

enum Direction
{
	up;
  down;
}

class Bolt: public Drawable
{
	public:
		Bolt(int y, int x)
		{
			this->y = y;
			this->x = x;
			this->icon = '*';
		}
};
