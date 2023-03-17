//header file for invadersWindows
//
#pragma once
#include "invaders.hpp"

#ifndef __spaceInvaders__invadersWindows__
#define __spaceInvaders__invadersWindows__

void initialize_windows();

void show_window(enum window_id);

void initialize_curses(void);

void create_window(WINDOW ** window, int height, int width, int y, int x);

void create_menu(WINDOW ** menuwin, int height, int width, int y, int x);
#endif /* defined(__spaceInvaders_invadersWindows__) */
