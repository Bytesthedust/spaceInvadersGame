//functions for game logic

#pragma once

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

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
			board.initialize();
			score = 0;
			scoreboard.initialize(score);


			lives = 3;
			livesBoard.initialize(lives);
		
			//initializes ship sprite
			createShip(SHIP_Y,SHIP_X);

			//initializes shield sprites
			//first shields
			
			createShields(4);
			createShields(4);

			//second shields
			createShields(20);
			createShields(20);
			
			//third shields
			createShields(36);
			createShields(36);

			//fourth shields
			createShields(52);
			createShields(52);
			
			displayShields(&shield);

			//create aliens
			createAlien(3, 6, 'A');//first row
					       //second row
					       //third row
					       //fourth row
			moveAliens(); //begin alien movement


		}


		void processInput()
		{
			chtype input = board.getInput();

			switch(input)
			{
				//player presses left arrow key to move left
				case KEY_LEFT:
					moveShip(left);
					break;

				//player presses right arrow key to move right
				case KEY_RIGHT:
					moveShip(right);
					break;

				//player presses spacebar to shoot bolt

				case ' ':
					createBolt(ship->getY()-1, ship->getX()); //initial position of the bolt in front of the ship
					moveBolt();
					break;

				//player presses p key to pause/unpause game
				case 'p':
					board.setTimeout(-1);
					while(board.getInput() != 'p');
					board.setTimeout(1000);
					break;
				case 'd':
					ship->getPosition(); //dev command. REMOVE when project is finished
					break;
			}
		}


		void updateState()
		{
			//logic for animation
 
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
		//void endgame();

	private:
		Board board;
		bool game_over;
		
		Ship* ship = NULL; //class for ship 
		Empty* space; //class for empty space
		Bolt* bolt = NULL; //class for bullet
		Bolt* a_bolt = NULL; //class for alien bullet
		Shield* shield = NULL; //class for shield object
		Alien* alien;

		Scoreboard scoreboard;
		int score;

		Lives livesBoard;
		int lives;

		//game logic functions


		//NOTE: find math function to randomize alien attacks
		//NOTE: because shields are linked lists, delete them the linked list way

		//SPRITE CREATION

		void createAlien(int y, int x, chtype icon)
		{
			alien = new Alien(y,x,icon);
			board.add(*alien);

		}

		//create ship icon	
		void createShip(int y, int x)
		{
			ship = new Ship(y, x);
			board.add(*ship);
		}

		void createBolt(int y, int x)
		{
			bolt = new Bolt(y, x);
			board.add(*bolt);
		}
		
		void createAlienBolt(int y, int x)
		{
			a_bolt = new Bolt(y,x);
			board.add(*a_bolt);
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
		}

		

		//SPRITE DESTRUCTION
		//when collision occurs, destroy bolt and sprite
		void destroySprite(int y, int x, int next_y)
		{
			board.addAt(y, x, ' '); //destroy bolt
			board.addAt(next_y, x, ' '); //destroy sprite
		}
		
		void destroyAlien(Alien** head, int y, int x);
		
		void destroyShip();
		
		void destroyShield(Shield** head, int y, int x); //work on first


		//SPRITE MOVEMENT
		//function to change position of ship icon
		void moveShip(Direction d)
		{

			//get current position of ship
			int ship_col = ship->getX();
			int ship_row = ship->getY();

			board.addAt(ship_row, ship_col, ' ');

			//BUG: ship is moving beyond game window(Resolved by adding a conditional to ship movement)

			//check if ship is moving beyond the window and update ship position

			switch(d)
			{
				case left:
					if(ship_col>1) 
						ship_col -= 1;
					break;

				case right:
					if(ship_col < 73)
						ship_col += 1;
					break;
				default:
					break;
			}

			//BUG: when ship moves, leaves trail of ghost ships.(Resolved by refreshing window)

			delete(ship); //memory cleanup of old ship
			createShip(ship_row, ship_col);
			board.refresh();
		
		}


		

		//BUG: when bolt fires, freezes ship in place
		void moveBolt()
		{
			//get the coordinates of the bolt
			int bolt_col = bolt->getX();

			int next_y = bolt->getY();

			//animating bolt movement

			for(next_y; next_y > 0; --next_y)
			{
				chtype ahead = board.getCharAt(next_y, bolt_col); //detect incoming sprite
				chtype input = board.getInput();

				//allows movement during bolt travel
				if(input == KEY_LEFT)
					moveShip(left);
				if(input == KEY_RIGHT)
					moveShip(right);

				//collision detection

				//if bolt hits shield
				if(ahead == '#')
				{
					destroySprite(bolt->getY(), bolt->getX(), next_y);
					delete(bolt);
					break;
				}

				//if bolt hits alien
				else if(ahead == 'A' || ahead == 'O' || ahead == 'G' || ahead=='X')
				{
					destroySprite(bolt->getY(), bolt->getX(), next_y);
					score += alien->getValue();
					scoreboard.updateScore(score);
					delete(bolt);
					break;
				}
				

				else
				{
					//else bolt continues to the border
					board.addAt(bolt->getY(), bolt->getX(), ' '); //create an empty space in the bolt old position
					delete(bolt); //memory management of bolt
					createBolt(next_y, bolt_col); //create a new bolt given the updated coordinates
				}
				
				board.refresh(); //refresh window to show bolt
				usleep(80000); //pause execution for 80000 miliseconds or 0.08 seconds. This simulates "flipping" to the next frame in the animation

			}

			if(next_y <= 0) //if bolt reaches border
			{
				board.addAt(bolt->getY(), bolt->getX(), ' ');
				delete(bolt);
			}


		}
		
		
		void moveAlienBolt()
		{
			//get the coordinates of the bolt
			createAlienBolt(alien->getY()+1, alien->getX());

			int bolt_col = a_bolt->getX();

			int next_y = a_bolt->getY();


			//animating bolt movement

			for(next_y; next_y > 0; ++next_y)
			{
				chtype ahead = board.getCharAt(next_y, bolt_col); //detect incoming sprite
				chtype input = board.getInput();

				//allows movement during bolt travel
				if(input == KEY_LEFT)
					moveShip(left);
				if(input == KEY_RIGHT)
					moveShip(right);

				//collision detection

				//if bolt hits shield
				if(ahead == '#')
				{
					destroySprite(a_bolt->getY(), a_bolt->getX(), next_y);
					delete(a_bolt);
					break;
				}

				//if bolt hits player
				else if(ahead == '^')//NOTE: Crash occurs
				{
					destroySprite(a_bolt->getY(), a_bolt->getX(), next_y);
					lives -= 1;
					livesBoard.updateLives(lives);
					delete(a_bolt);
					break;
				}

				else
				{

					//else bolt continues to the border
					board.addAt(a_bolt->getY(), a_bolt->getX(), ' '); //create an empty space in the bolt old position
					delete(a_bolt); //memory management of bolt
					createAlienBolt(next_y, bolt_col); //create a new bolt given the updated coordinates
					if(next_y > 29)
					{
						board.addAt(a_bolt->getY(), a_bolt->getX(), ' ');
						delete(a_bolt);
						break;
					}
				}
				board.refresh(); //refresh window to show bolt
				usleep(80000); //pause execution for 80000 miliseconds or 0.08 seconds. This simulates "flipping" to the next frame in the animation

			}


		}


		void moveAliens()
		{
			//all aliens move at the same speed. Probably sleep the loop
			//aliens start at the left side of the screen and move right
			//if aliens reach the border of screen, descend the screen by one and move in the opposite direction
			int alien_col = alien->getX();
			int alien_row = alien->getY();
			int direction = 0; //0 = right, 1 = left
			srand(1978);


			while(alien_row < 29 && alien->getIcon() != ' ')
			{
				int shoot = rand() % 2;
				chtype input = board.getInput();

				//NOTE: SLOW
				/*
				if(input == KEY_LEFT)
					moveShip(left);
				if(input == KEY_RIGHT)
					moveShip(right); */

				if(direction == 0) //move right
				{
					alien_col += 1;
				}

				else if(direction == 1) //move left
				{
					alien_col -= 1;
				}

				if(alien_col > 73)
				{
					alien_col -= 1;
					alien_row += 1;
					direction = 1;
				}

				if(alien_col < 1)
				{
					alien_col += 1;
					alien_row += 1;
					direction = 0;
				}

				if(shoot == 1)
					moveAlienBolt();

				board.addAt(alien->getY(), alien->getX(), ' ');
				createAlien(alien_row, alien_col, 'A');
				board.refresh();
				usleep(800000);
			}


		}




};
