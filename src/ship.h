// class to represent player character

#pragma once
#include <ncurses.h>
#include "drawable.hpp"

//directions that the ship can move in
enum Direction
{
	left = 2;
	right = 2;
};

class Ship: public Drawable
{
	public:
		Ship(int y, int x)
		{
			this->y = y;
			this->x = x;
			this->icon = '^';
		}
};
