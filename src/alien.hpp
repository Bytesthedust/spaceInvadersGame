//class to represent aliens
//


#pragma once
#include <ncurses.h>
#include "drawable.hpp"
#include "invaders.hpp"


class Alien: public Drawable
{

	Alien* next;
	int value;

	public: 

		Alien(int y, int x, chtype icon)
		{
			this->y = y;
			this->x = x;
			this->icon = icon;
			next = NULL;

			switch(icon)
			{
				case 'A':
					this->value = 40;
					break;
				case 'O':
					this->value = 30;
					break;
				case 'G':
					this->value = 20;
					break;
				case 'X':
					this->value = 10;
					break;
				default:
					this->value = 0;
					break;
			}

		}

		//get alien value
		int getValue()
		{
			return value;
		}

		//push function

};

