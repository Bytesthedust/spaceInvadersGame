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
			
			ship.setDirection(right);
		}


		void processInput()
		{
			chtype input = board.getInput();

			switch(input)
			{
				case KEY_LEFT:
					//action
					ship.setDirection(left);
					break;

				case KEY_RIGHT:
					ship.setDirection(right);
					break;

				//NOTE: case for spacebar (shoot bolt)

				/*case ' ':
					ship.shootBolt();
					break;*/

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
			
			//NOTE: SHIP not appearing on screen
			ship.move();

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
		Ship ship;

		Scoreboard scoreboard;
		int score;

		Lives livesBoard;
		int lives;

};
