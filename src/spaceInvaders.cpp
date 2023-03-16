#include <ostream>
#include <iostream>
#include <ncurses.h>

using namespace std;


int main(int argc, char** argv)
{
	//ncurses start
	
	//NOTE: x coordinates increase to the right, y coordinates increase fom top to bottom
	initscr();
	noecho();
	cbreak();
	curs_set(0);

	//get screen size
	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);

	//window for input
	
	WINDOW * menuwin = newwin(16, xMax=30, yMax=8, 15);
	box(menuwin,0,0);
	refresh();
	wrefresh(menuwin);
	keypad(menuwin, true); //makes it so we can use arrow keys

	//menu options
	string choices[2] = {"Start", "Quit"};
	int choice;
	int highlight = 0;

	while(1)
	{
		for(int i = 0; i < 2; i++) //print menu and highlight current choice
		{
			if(i == highlight)
			{
				wattron(menuwin, A_REVERSE);
			}
			mvwprintw(menuwin, i+1, 1, choices[i].c_str());
			wattroff(menuwin, A_REVERSE);
		}

		choice = wgetch(menuwin);

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
			break;
		}
	}
	//program waits before exiting
	getch();
	endwin();

	return 0;

}
