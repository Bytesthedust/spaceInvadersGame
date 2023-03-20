//class for player lives window
//

#pragma once

#include <ncurses.h>

class Lives
{
	WINDOW *lives_win;

	public:
		Lives()
		{
		}

		Lives(int width, int y, int x)
		{
			lives_win = newwin(1, width, y, x);
		}

		void initialize(int initial_lives)
		{
			clear();
			mvwprintw(lives_win,0,0,"Lives:");
			updateLives(initial_lives);
			refresh();
		}

		void updateLives(int lives)
		{
			mvwprintw(lives_win,0, lives_win->_maxx-10, "%11llu", lives);
		}

		void clear()
		{
			wclear(lives_win);
		}

		void refresh()
		{
			wrefresh(lives_win);
		}
};
