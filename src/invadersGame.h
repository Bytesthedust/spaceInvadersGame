//functions for game logic

#pragma once

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>

#include "invadersBoard.hpp"
#include "drawable.hpp"
#include "ship.hpp"
#include "alien.hpp"
#include "empty.hpp"
#include "invadersScore.hpp"
#include "lives.hpp"
#include "bolt.hpp"
#include "shield.hpp"

class invadersGame
{
	public:
		invadersGame(int height, int width, int speed = 300)
		{
			board = Board(height, width, speed);

			int sb_row = board.getStartRow() + height;
			int sb_col = board.getStartCol();

			int lv_row = board.getStartRow() + height+1;
			int lv_col = board.getStartCol();

			scoreboard = Scoreboard(width, sb_row, sb_col);
			livesBoard = Lives(width, lv_row, lv_col);

			initialize();
		}

		void initialize()
		{
			board.initialize(); //create game board
			score = 0;
			scoreboard.initialize(score);


			lives = 3;
			livesBoard.initialize(lives);
		
			//initializes ship sprite
			createShip(SHIP_Y,SHIP_X);

			//initializes shield sprites
			
			//first shields
			createShields(4);

			//second shields
			createShields(20);
			
			//third shields
			createShields(36);

			//fourth shields
			createShields(52);
			
			displayShields(&shield); //add shields to board

			//create aliens
			createAlien(3, 6, 'A'); //first row
			createAlien(5, 6, 'O'); //second row
			createAlien(7, 6, 'G'); //third row
			createAlien(9, 6, 'X'); //fourth row

			displayAliens(alien); //add aliens to board

		}


		chtype processInput()
		{
			chtype input = board.getInput();

			return input;
		}


		void updateState(unsigned tick)
		{
			//logic for animation->update positions for sprites at the same time + usleep in main loop
			//60 fps => 1000/fps = 16.6 fpms

			chtype input = processInput();

			//update ship position
			if(input == KEY_LEFT || input == KEY_RIGHT)
				updateShip(input);

			//fire bolt if spacevar is pressed
			if(input == ' ')
				createBolt(ship->getY()-1, ship->getX());
			updateBolt();
			
			//alien movement
			if(tick % 1 == 0)
			{
				updateAlien(alien);
				alien = alien->next;
			}
 
		}

		void redraw()
		{
			board.refresh();
			scoreboard.refresh();
			livesBoard.refresh();
		}

		bool isOver()
		{
			return game_over;
		}

		int getScore()
		{
			return score;
		}

		int getLives()
		{
			return lives;
		}

		//checks for game over conditions
		bool endgame()
		{
			if(lives == 0)
			{
				game_over = true;
			}

			return game_over;
		}


	private:
		Board board;
		bool game_over;
	
		
		
		int shift = 1; //alien x shift

		int yshift = -1;

		Ship* ship = NULL; //class for ship 
		Empty* space; //class for empty space
		Bolt* bolt = NULL; //class for bullet
		Bolt* bomb = NULL; //class for alien bullet
		Shield* shield = NULL; //class for shield object
		
		Alien* alien = NULL;

		Scoreboard scoreboard;
		int score;

		Lives livesBoard;
		int lives;

		//game logic functions

		//SPRITE CREATION
		void createAlien(int y, int x, chtype icon)
		{

			int rowLimit = x + 10;

			for(x; x < rowLimit; ++x)
			{

				alien = alien->addEnd(alien,y,x*2,icon);
			}

		}

		//create ship sprite
		void createShip(int y, int x)
		{
			ship = new Ship(y, x);
			board.add(*ship);
		}

		//create bolt sprite
		void createBolt(int y, int x)
		{
			bolt = new Bolt(y, x);
			board.add(*bolt);
		}
		
		//create alien bomb sprite
		void createAlienBolt(int y, int x)
		{
			bomb = new Bolt(y,x);
			board.add(*bomb);
		}
		
		//function to create 2 rows of shields
		void createShields(int startX)
		{
		
			int rowlimit = startX + 8;

			for(startX; startX < rowlimit; ++startX)
			{
				shield->push(25, startX, &shield);
				shield->push(26, startX, &shield);
			}

		}

		void displayShields(Shield** head)
		{
			Shield* temp = *head;
			while(temp->next != NULL)
			{
				board.add(*temp);
				board.refresh();
				temp = temp->next;
			}
			board.add(*temp);
			board.refresh();
		}

		//show aliens on board
		void displayAliens(Alien* head)
		{
			Alien* temp = head;
			do
			{
				board.add(*temp);
				temp = temp->next;
			} while(temp != head);
		}


		//update functions

		//update ship position
		void updateShip(chtype ch)
		{
			int ship_col = ship->getX();
			int ship_row = ship->getY();

			board.addAt(ship_row, ship_col, ' ');

			switch(ch)
			{
				case KEY_LEFT:
					if(ship_col > 1)
					{
						ship_col -= 1;
						ship->setX(ship_col);
					}

					break;
				case KEY_RIGHT:
					if(ship_col < 73)
					{
						ship_col += 1;
						ship->setX(ship_col);
					}

					break;
				default:
					break;
			}

			board.add(*ship);

		}

		//update bolt position
		void updateBolt()
		{
			if(bolt == NULL)
				return;

			int bolt_col = bolt->getX();
			int bolt_row = bolt->getY();

			
			if(bolt_row == 1)
			{
				board.addAt(bolt_row, bolt_col, ' ');
				bolt = NULL;
				return;
			}

			//collision detection

			
			//bolt hits alien
			if(alien->getY() == bolt->getY() && alien->getX() == bolt->getX())
			{
				board.addAt(alien->getY(), alien->getX(), ' ' );
				alien->setIcon(' ');
				board.addAt(bolt_row, bolt_col, ' ');
				score += alien->getValue(alien->getIcon());
				scoreboard.updateScore(score);
			}


			else
			{
				board.addAt(bolt_row, bolt_col, ' ');
				bolt_row -= 1;
				bolt->setY(bolt_row);
				board.add(*bolt);
			}
		}

		//update alien position
		void updateAlien(Alien* head)
		{


			board.addAt(head->getY(), head->getX(), ' ');

			int next_x = head->getX() + shift;
			//int next_y = head->getY() + 1; //NOTE: Find way to shift every alien down once border is reached

			if(next_x >= 35 || next_x <= 1)
			{
				shift *= -1;
				int next_y = head->getY() + 1;
				if(head->getY() < next_y)
					head->setY(next_y);
			}

			else
			{

				head->setX(next_x);
				//head->setY(next_y);
			}



			board.add(*head);


		}

		//update bomb position
		void updateBomb()
		{
			int bomb_col = bomb->getX();
			int bomb_row = bolt->getY();

			if(bomb_row >= 30)
				return;

			else
			{
				bomb_row += 1;
				bomb->setY(bomb_row);
				board.addAt(--bomb_row, bomb_col, ' ');
			}

		}	
		

};
