//functions to create shield objects

#pragma once
#include <ncurses.h>
#include "drawable.hpp"

class Shield: public Drawable
{
	public:
		Shield* head;
		Shield* next;
		bool isDestroyed;


		Shield(int y, int x)
		{
			this->y=y;
			this->x=x;
			this->icon = '#';
			isDestroyed = false;
			next = NULL;

		}

		void setY(int newY)
		{
			y=newY;
		}

		void setX(int newX)
		{
			x=newX;
		}

		void setStatus(int check)
		{
			if(check == 0)
				isDestroyed = false;
			else if(check == 1)
				isDestroyed = true;
			else
				isDestroyed;
		}

		void push(int y, int x, Shield** head)
		{
			Shield* newShield = new Shield(y, x);
			Shield* temp = *head;
			
			if(*head == NULL)
			{
				*head = newShield;
				return;
			}

			while(temp->next != NULL)
			{
				temp = temp->next;
			}

			temp->next = newShield;
			return;

		}

};
