#include <ncurses.h>
#include "invadersWindows.h"

//initialize ncurses
void initialize_curses()
{
  initscr(); //start ncurses
  cbreak(); //disable line buffer
  noecho(); //input will not be echoed
  
  keypad(stdscr, true); //enable keyboard
  
  curs_set(0); //switch off cursor
  
  refresh() //draw stdscr
}

//create window

void create_window(WINDOW ** window, int height, int width, int y, int x)
{
  //create window
  *window = newwin(height, width, y, x);
  
  keypad(*window, true); //read input from keyboard
  
  //display border around window
  box(*window, 0 ,0);
}
