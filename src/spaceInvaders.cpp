#include <ostream>
#include <iostream>
#include <ncurses.h>
#include "invadersWindows.hpp"
#include "invaders.hpp"
#include "invadersGame.hpp"
#include "drawable.hpp"
#include "invadersBoard.hpp"



using namespace std;


int main(int argc, char** argv)
{
	//ncurses start
	
	//NOTE: x coordinates increase to the right, y coordinates increase fom top to bottom
	
	initialize_curses(); //start ncurses
	
	invadersGame game(WIN_GAME_HEIGHT, WIN_GAME_WIDTH, 180);

	while(1)
	{
		game.processInput();
		game.updateState();
		game.redraw();
	}

	//initialize_windows();

	//show_window(WIN_MENU);


	//program waits before exiting
	endwin();

	return 0;

}
