//class to simulate empty space in the game

#pragma once
#include <ncurses.h>
#include "drawable.hpp"

class Empty: public Drawable
{
	public:
		Empty(int y, int x)
		{
			this->y = y;
			this->x = x;
			this->icon = ' '; //symbol for empty space

		}
};
