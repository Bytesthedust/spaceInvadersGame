//functions for game logic

#pragma once

#include <ncurses.h>

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
					createBolt(ship->getY()-1, ship->getX());
					moveBolt(bolt->getY());
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


	
		//helper functions	
		void createShip(int y, int x)
		{
			ship = new Ship(y, x);
			board.add(*ship);
		}

		void destroyShip(int y, int x)
		{
			delete ship;
			space = new Empty(y, x);
			board.add(*space);
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

		//NOTE: Bolt not moving-> when bolt fires, freezes ship
		void moveBolt(int next_y)
		{
			int bolt_col = bolt->getX();
			//int next_y = bolt_row-1;

			while(next_y > 0)
			{

				board.clear(); //NOTE: board.clear() clears whole screen but leaves bolt


			//find math function used to randomize attacks
				bolt->setY(next_y);
				createBolt(next_y, bolt_col);
				next_y -= 1;
			}

			
		}

		//function to create 2 rows of shields
		void createShields(int startY, int startX)
		{
			for(startY; startY < sizeof(shield); startY++)
			{
				for(startX; startX < 4; startX++)
				{
					*shield[startY, startX] = new Shield(startY, startX);
					board.add(**shield[startY, startX]);
					board.refresh();
					
				}
			}
		}




};
