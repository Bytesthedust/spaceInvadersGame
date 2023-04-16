//class to represent aliens
//


#pragma once
#include <ncurses.h>
#include "drawable.hpp"
#include "invaders.hpp"


class Alien: public Drawable
{


	public:
       		Alien* next;

		Alien(int y, int x, chtype icon)
		{
			this->y = y;
			this->x = x;
			this->icon = icon;
			next = NULL;

		}

		//get alien value
		int getValue(chtype ahead)
		{
			if(ahead == 'A')
				return 40;
			else if(ahead == 'O')
				return 30;
			else if(ahead == 'G')
				return 20;
			else if(ahead == 'X')
				return 10;

			return 0;
		}


		//push function
		void push(int y, int x, chtype icon, Alien** head)
		{
			Alien* newAlien = new Alien(y, x, icon);
			Alien* temp = *head;

			newAlien->next = NULL;

			if(*head == NULL)
			{
				*head = newAlien;
				return;
			}

			while(temp->next != NULL)
			{
				temp = temp->next;
			}

			temp->next = newAlien;
			return;
		}

};



