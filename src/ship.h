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


class Ship: public Drawable
{
	Direction cur_direction;
	bool isDestroyed;
	public:
		Ship()
		{
			this->x=this->y=0;
			this->icon = '^';
		}

		Ship(int y, int x)
		{
			this->x = x;
			this->y = y;
			this->icon = '^';
		}


		Direction getDirection()
		{
			return cur_direction;
		}

		void setDirection(Direction d)
		{
			cur_direction = d;
		}

		void setY(int newY)
		{
			y = newY;
		}

		void setX(int newX)
		{
			if(newX <= WIN_GAME_WIDTH)
				
			{
				this->x = newX;
			}
			
			else if(newX <=73)
			{
				this->x = newX;
			}

			else
			{
				x = 10;
			}
			//x = newX<=10?10:newX;
		}

		//development method. REMOVE when FINISHED
		void getPosition()
		{
			std::cout <<"Y: " << y <<"X: " << x << std::endl;
		}

		//shootBolt method: spawn in front of ship. While bolt is in empty space, continue forward. If bolt hits non-empty space, destroy object and bolt

		
};
