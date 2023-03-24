//function for game logic

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
		
			createShip(SHIP_Y,SHIP_X);

		}


		void processInput()
		{
			chtype input = board.getInput();

			switch(input)
			{
				case KEY_LEFT:
					//action
					//ship.setDirection(left);
					moveShip(left);
					break;

				case KEY_RIGHT:
					//ship.setDirection(right);
					moveShip(right);
					break;

				//NOTE: case for spacebar (shoot bolt)

				case ' ':
					createBolt(ship->getY()-1, ship->getX());
					break;

				case 'p':
					//enable pausing of the game
					board.setTimeout(-1);
					while(board.getInput() != 'p');
					board.setTimeout(1000);
					break;
			}
		}


		void updateState()
		{
			//logic for animation
			
			bolt.clear();
 
			

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
		bool isDestroyed; //boolean to check if object has been destroyed
		Ship* ship; //class for ship 
		Empty* space; //class for empty space
		Bolt* bolt; //class for bullet

		Scoreboard scoreboard;
		int score;

		Lives livesBoard;
		int lives;

		
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
			//moveBolt();
		}

		//NOTE: Bolt not moving
		void moveBolt()
		{
			int bolt_col = bolt->getX();
			int bolt_row = bolt->getY();
			while(1)
			{
				bolt->setY(--ship->getY());
				board.clear();
				createBolt(bolt->getY(), bolt_col);
			}
		}


};
