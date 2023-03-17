#include <ostream>
#include <iostream>
#include <ncurses.h>

#include "invadersWindows.hpp"
#include "invaders.hpp"

using namespace std;


int main(int argc, char** argv)
{
	
	
	//start ncurses
	initialize_curses();
	
	//create windows
	initialize_windows();
	
	//show main menu
	show_window(WIN_MENU);

	//get screen size
	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	
	
	//program waits before exiting
	getch();
	endwin();

	return 0;

}
