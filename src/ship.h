// class to represent player character

#pragma once
#include <ncurses.h>
#include "drawable.hpp"
#include "invaders.hpp"
//#include "bolt.hpp"

//directions that the ship can move in
enum Direction
{
	left,
	right
};

class Sprite: public Drawable
{
	public:
		Sprite()
		{
			this->x = this->y = 0;
			this->icon = '^';
		}

		Sprite(int y, int x)
		{
			this->x = x;
			this->y = y;
			this->icon = '^';
		}

};


class Ship: Sprite
{
	Sprite shipSprite;
	Direction cur_direction;
	public:
		Ship()
		{
			cur_direction = right;
		}

		Direction getDirection()
		{
			return cur_direction;
		}

		void setDirection(Direction d)
		{
			cur_direction = d;
		}

		//shootBolt method: spawn in front of ship. While bolt is in empty space, continue forward. If bolt hits non-empty space, destroy object and bolt

		Sprite move()
		{
			int row = shipSprite.getY();
			int col = shipSprite.getX();

			switch(cur_direction)
			{
				case left:
					col--;
					break;
				case right:
					row--;
					break;
				default:
					break;
			}

			return Sprite(row, col);
		}
};
