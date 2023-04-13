//functions to create shield objects

#pragma once
#include <ncurses.h>
#include "drawable.hpp"

class Shield: public Drawable
{
	public:

		Shield* next;
		bool isHead;

		Shield(int y, int x)
		{
			this->y=y;
			this->x=x;
			this->icon = '#';
		}

		
		//add new shield to list

		void push(int y, int x, Shield** head)
		{
			Shield* newShield = new Shield(y,x);
			Shield* temp = *head;

			newShield->next = NULL;

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

		Shield* deleteHead(Shield* head)
		{
			if(head == NULL)
				return NULL;

			Shield* temp = head;
			head = head->next;

			delete temp;
			return head;
		}

		Shield* deleteTail(Shield* head)
		{
			if(head == NULL)
				return NULL;
			if(head->next = NULL)
			{
				delete head;
				return NULL;
			}

			Shield* second_last = head;
			while(second_last->next->next != NULL)
				second_last = second_last->next;

			delete(second_last);

			second_last->next = NULL;
			return head;
		}

		/*
		Shield* removeNode(Shield** head)
		{
			Shield* temp = *head;
			while(temp->next != NULL)
			{
				if(temp->getIcon() == ' ')
				{
*/

		/*
		void deleteGivenNode(Shield** head, int y, int x)
		{
			if(*head == NULL)
				return;

			Shield* temp = *head;

			if(

		*/


};
