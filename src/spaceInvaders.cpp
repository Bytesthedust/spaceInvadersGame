#include <ostream>
#include <iostream>
#include <ncurses.h>
#include "invadersWindows.hpp"
#include "invaders.hpp"
#include "invadersGame.hpp"
#include "drawable.hpp"
#include "invadersBoard.hpp"

//main menu window dimensions

//WINDOW* menuwin;

using namespace std;



int main(int argc, char** argv)
{
	//ncurses start
	
	
	//NOTE: x coordinates increase to the right, y coordinates increase fom top to bottom
	
	initialize_curses(); //start ncurses

	//start menu
	/*WINDOW* menuwin;
	create_menu(&menuwin, WIN_MENU_HEIGHT, WIN_MENU_WIDTH, WIN_MENU_Y, WIN_MENU_X);

	wclear(menuwin);
	usleep(8000);*/
	
	unsigned tick = 1;

	invadersGame game(WIN_GAME_HEIGHT, WIN_GAME_WIDTH, 17); //original: 180, dev: 17

	while(1)
	{
		game.processInput();
		game.updateState(tick); //additional update functions: bolt, bomb, alien
		game.redraw();
		//clock for game
		tick = (tick > 60) ? 0 : tick + 1;
		napms(1);
	}



	//program waits before exiting
	endwin();

	return 0;

}
