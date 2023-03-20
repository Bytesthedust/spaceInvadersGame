#pragma once

#include <ncurses.h>

class Scoreboard
{
	WINDOW *score_win;

	public:
		Scoreboard()
		{
		}

		Scoreboard(int width, int y, int x)
		{
			score_win = newwin(1, width, y, x);
		}

		void initialize(int initial_score)
		{
			clear();
			mvwprintw(score_win,0,0,"Score:");
			updateScore(initial_score);
			refresh();
		}

		void updateScore(int score)
		{
			mvwprintw(score_win, 0, score_win->_maxx-10, "%11llu", score); //allows a space for 11 characters

		}


		void clear()
		{
			wclear(score_win);

		}

		void refresh()
		{
			wrefresh(score_win);
		}

};
