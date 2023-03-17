//functions for managing the windows in space invaders
//

#include <string>
#include <ostream>
#include <iostream>
#include <ncurses.h>
#include <panel.h>
#include "invadersWindows.hpp"
#include "invaders.hpp"

static struct invaderWindow invaderwindows[] = {
	{WIN_MENU, WT_MENU, NULL}, //main menu window
	{WIN_GAME, WT_GAME, NULL}, //game window
	{WIN_OVER, WT_OVER, NULL} //game over window
};

void initialize_windows()
{
	for(int i =0; i < ARRAY_SIZE(invaderwindows); i++)
	{
		switch (invaderwindows[i].w_cat)
		{
			case WT_MENU:
				create_menu(&invaderwindows[i].w_ptr, WIN_MENU_HEIGHT, WIN_MENU_WIDTH, WIN_MENU_Y, WIN_MENU_X);
				break;
			case WT_GAME:
				create_window(&invaderwindows[i].w_ptr, WIN_GAME_HEIGHT, WIN_GAME_WIDTH, WIN_GAME_Y, WIN_GAME_X);
				break;
			case WT_OVER:
				create_window(&invaderwindows[i].w_ptr, WIN_OVER_HEIGHT, WIN_OVER_WIDTH, WIN_OVER_Y, WIN_OVER_X);
				break;
			default:
				break;
		}
	}
}

//returns a pointer to struct that matches the uid or NULL if no match is found

struct invaderWindow * get_invaderWindow(enum window_id win_id)
{
	for(int i = 0; i < ARRAY_SIZE(invaderwindows); i++)
	{
		if(invaderwindows[i].uid == win_id)
		{
			return &invaderwindows[i];
		}
	}

	return NULL;
}

//show specified window
void show_window(enum window_id win_id)
{
	struct invaderWindow * invader_win = get_invaderWindow(win_id);

	//update the window
	wrefresh(invader_win->w_ptr);

	doupdate();
}

//initialize curses
void initialize_curses(void)
{
	initscr(); //start ncurses
	cbreak(); //disable line buffeer
	noecho(); //input will not be echoed

	keypad(stdscr, true); //enable keyboard

	curs_set(0); //switch off the cursor

	refresh(); //draw stdscr

}

//create a window with associated panel and border

//removed parameter: PANEL ** panel
void create_window(WINDOW ** window, int height, int width, int y, int x)
{
	//create window
	*window = newwin(height, width, y, x);

	keypad(*window, true); //read input from keyboard

	//add window to a panel
	//*panel = new_panel(*window);

	//display border around window
	box(*window, 0, 0);
}

void create_menu(WINDOW ** menuwin, int height, int width, int y, int x)
{
	create_window(menuwin, height, width, y, x);

	//menu options
	std::string choices[2] = {"Start", "Quit"};
	int choice;
	int highlight = 0;

	while(1)
	{
		for(int i = 0; i < 2; i++) //print menu and highlight current choice
		{
			if(i == highlight)
			{
				wattron(*menuwin, A_REVERSE);
			}
			mvwprintw(*menuwin, i+1, 1, choices[i].c_str());
			wattroff(*menuwin, A_REVERSE);
		}

		choice = wgetch(*menuwin);

		switch(choice)
		{
			case KEY_UP:
				highlight--;
				if(highlight == -1) //checks if user is going out of bounds
					highlight = 1;
				break;
			case KEY_DOWN:
				highlight++;
				if(highlight == 2)
					highlight = 0;
				break;
			default:
				break;
		}

		if(choice == 10) //if user presses enter
		{
			if(highlight == 0)
			{
				show_window(WIN_GAME);
			}

			else
			{
				break;
			}
			
		}
	}

	wrefresh(*menuwin);
}

