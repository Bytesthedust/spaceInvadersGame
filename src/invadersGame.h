//functions for game logic

#pragma once

#include <ncurses.h>
#include <unistd.h>

#include "invadersBoard.hpp"
#include "drawable.hpp"
#include "ship.hpp"
//#include "alien.hpp"
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
			createShields(25,4);


			//create aliens

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

	private:
		Board board;
		bool game_over;
		
		Ship* ship; //class for ship 
		Empty* space; //class for empty space
		Bolt* bolt; //class for bullet
		Shield* shield[2][4]; //class for shield object

		Scoreboard scoreboard;
		int score;

		Lives livesBoard;
		int lives;

		//NOTE: find math function to randomize alien attacks

	
		//helper functions	
		void createShip(int y, int x)
		{
			ship = new Ship(y, x);
			board.add(*ship);
		}


		//function to change position of ship icon
		void moveShip(Direction d)
		{

			//get current position of ship
			int ship_col = ship->getX();
			int ship_row = ship->getY();

			//BUG: ship is moving beyond game window(Resolved by adding a conditional to ship movement)

			//check if ship is moving beyond the window

			switch(d)
			{
				case left:
					if(ship_col>1) 
						ship->setX(--ship_col);
					break;

				case right:
					if(ship_col < 73)
						ship->setX(ship_col++);
					break;
				default:
					break;
			}

			//BUG: when ship moves, leaves trail of ghost ships.(Resolved by refreshing window)


			board.clear();
			createShip(ship->getY(), ship_col);
		
		}

		void createBolt(int y, int x)
		{

			bolt = new Bolt(y, x);
			board.add(*bolt);
		}

		//BUG: when bolt fires, freezes ship in place
		void moveBolt()
		{
			//get the coordinates of the bolt
			int bolt_col = bolt->getX();

			int next_y = bolt->getY();

			//animating bolt movement
			for(next_y; next_y > 0; --next_y) //NOTE: ADD conditions if a collision with an alien or shield occurs
			{

				board.addAt(bolt->getY(), bolt->getX(), ' '); //create an empty space in the bolt old position
				bolt->setY(next_y); //set the new coordinate for bolt
				createBolt(next_y, bolt_col); //create a new bolt given the updated coordinates
				
				board.refresh(); //refresh window to show bolt
				usleep(80000); //pause execution for 80000 miliseconds or 0.08 seconds. This simulates "flipping" to the next frame in the animation

			}

			if(next_y <= 0) //erase bolt if it reaches the border
			{
				board.addAt(bolt->getY(), bolt->getX(), ' ');
			}


		}

		//function to create 2 rows of shields
		//NOTE: Shields are not appearing
		void createShields(int startY, int startX)
		{
			for(startY; startY < sizeof(shield); ++startY)
			{
				for(startX; startX < 4; ++startX)
				{
					*shield[startY, startX] = new Shield(startY, startX);
					board.add(**shield[startY, startX]);
				}
			}
		}




};
