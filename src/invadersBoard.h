#pragma once

//create game board
#include <cstdlib>
#include <stdio.h>
#include "drawable.hpp"
#include "time.hpp"
#include "invaders.hpp"

class Board
{
	public:
		Board()
		{
			construct(WIN_GAME_HEIGHT, WIN_GAME_WIDTH, 300);
		}

		Board(int height, int width, int speed)
		{
			construct(height, width, speed);
		}

		void initialize()
		{
			clear();
			refresh();
		}


		void addBorder()
		{
			box(board_win, 0, 0);
		}

		void add(Drawable drawable) //creates view or an interface to display information
		{
			addAt(drawable.getY(), drawable.getX(), drawable.getIcon());
		}

		void addAt(int y, int x, chtype ch)
		{
			mvwaddch(board_win, y, x, ch); //adds character to window then advances cursor
		}

		chtype getInput()
		{
			time_t time_last_input = Time::milliseconds();

			chtype input = wgetch(board_win); //waits for a set time between inputs
			chtype new_input = ERR; //default value of wgetch

			setTimeout(0);
			while(time_last_input + timeout >= Time::milliseconds())
			{
				new_input = wgetch(board_win); //allows change of direction without being locked in

			}

			setTimeout(timeout); //reset to previous time

			if(new_input != ERR) //if the input has been changed before time elapsed
			{
				input = new_input;
			}

			return input;
		}

		chtype getCharAt(int y, int x) //returns the position of a char. Aids collision detection
		{
			return mvwinch(board_win, y, x);
		}

		void clear()
		{
			wclear(board_win); //clears window

			addBorder(); //restores border
				     
		}

		void refresh() //refreshes window
		{
			wrefresh(board_win);
		}

		void setTimeout(int timeout)
		{
			wtimeout(board_win, timeout);
		}

		int getTimeout()
		{
			return timeout;
		}

		int getStartRow()
		{
			return start_row;
		}

		int getStartCol()
		{
			return start_col;
		}

	private:
		WINDOW* board_win;

		int height, width, start_row, start_col, timeout;

		void construct(int height, int width, int speed)
		{
			int xMax, yMax;
			getmaxyx(stdscr, yMax, xMax);

			this->height = height;
			this->width = width;

			start_row = (yMax/2) - (height/2);
			start_col = (xMax/2) - (width/2);

			board_win = newwin(height, width, start_row, start_col);

			//timeout
			timeout = speed;

			setTimeout(speed); //refresh the board when elapsed. Smaller time = faster animation
			keypad(board_win, true); //captures arrow key input
			
		}
};
