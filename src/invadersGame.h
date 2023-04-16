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
			
			displayShields(&shield);

			//create aliens
			createAlien(3, 6, 'A'); //first row
			createAlien(5, 6, 'O'); //second row
			createAlien(7, 6, 'G'); //third row
			createAlien(9, 6, 'X'); //fourth row


			displayAliens(&alien);
			//moveAlien(); //begin alien movement

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
		
		Ship* ship = NULL; //class for ship 
		Empty* space; //class for empty space
		Bolt* bolt = NULL; //class for bullet
		Bolt* a_bolt = NULL; //class for alien bullet
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

			int rowLimit = x + 6;
			for(x; x < rowLimit; ++x)
			{
				alien->push(y,x,icon,&alien);
			}

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
			board.add(*temp);
			board.refresh();
		}

		void displayAliens(Alien** head)
		{
			Alien* temp = *head;
			while(temp->next != NULL)
			{
				board.add(*temp);
				board.refresh();
				temp = temp->next;
			}

			board.add(*temp);
			board.refresh();
		}

	

		//SPRITE DESTRUCTION
		//when collision occurs, destroy bolt and sprite
		void destroySprite(int y, int x, int next_y)
		{
			board.addAt(y, x, ' '); //destroy bolt
			board.addAt(next_y, x, ' '); //destroy sprite
		}

		//ship takes damage 
		void shipHit()
		{
			board.addAt(ship->getY(), ship->getX(), ' ');
			delete(ship);
			lives -= 1; //player loses a life
			livesBoard.updateLives(lives);
			createShip(SHIP_Y, SHIP_X); //respawn ship at default location
		}
		

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
					score += alien->getValue(ahead);
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
		
		//handles all mechanics for alien enemies
		void moveAlien()
		{
			//get position of alien
			int alien_col = alien->getX();
			int alien_row = alien->getY();
			int direction = 0; //flag to determine alien direction
			
			srand(time(0)); //seed for shoot variable random value

			int shoot; // flag for shooting
			
			//begin movement
			while(alien_row < 29)
			{
				shoot = rand() % 2;
				//implement player movement

				if(direction ==  0) //move right
				{
					alien_col += 1;
					if(alien_col > 73)
					{
						alien_col -= 1;
						alien_row += 1;
						direction = 1;
					}

					//add shoot mechanic
					if(shoot == 1)
						moveAlienBolt();
				}

				else if(direction == 1) // move left
				{
					alien_col -= 1;
					if(alien_col < 1)
					{
						alien_col += 1;
						alien_row += 1;
						direction = 0;
					}

					//add shoot mechanic
					if(shoot == 1)
						moveAlienBolt();
				}

				//animation
				board.addAt(alien->getY(), alien->getX(), ' ');
				createAlien(alien_row, alien_col, 'A'); //change to get icon of alien
				board.refresh();
				usleep(800000);


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

				/*
				//allows movement during bolt travel
				if(input == KEY_LEFT)
					moveShip(left);
				if(input == KEY_RIGHT)
					moveShip(right);
					*/

				//processInput();


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
					board.addAt(a_bolt->getY(), a_bolt->getX(), ' ');
					delete(a_bolt);
					shipHit();
					break;
				}

				else
				{

					//else bolt continues to the border
					board.addAt(a_bolt->getY(), a_bolt->getX(), ' '); //create an empty space in the bolt old position
					delete(a_bolt); //memory management of bolt	
					createAlienBolt(next_y, bolt_col); //create a new bolt given the updated coordinates
				
				}
				board.refresh(); //refresh window to show bolt
				usleep(80000); //pause execution for 80000 miliseconds or 0.08 seconds. This simulates "flipping" to the next frame in the animation

			}
			if(a_bolt->getY() > 28)
			{
				board.addAt(a_bolt->getY(), a_bolt->getX(), ' ');
			}


		}

};
