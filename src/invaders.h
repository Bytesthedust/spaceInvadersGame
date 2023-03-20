//header files containing common definitions and values
//

#pragma once

//macro to find size of array

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

//main menu window dimensions

#define WIN_MENU_HEIGHT 16
#define WIN_MENU_WIDTH 15
#define WIN_MENU_Y 8
#define WIN_MENU_X 30

//game window dimensions
#define WIN_GAME_HEIGHT 20 * 2
#define WIN_GAME_WIDTH 25 * 4
#define WIN_GAME_Y 12
#define WIN_GAME_X 30

//game over window dimensions

#define WIN_OVER_HEIGHT 16
#define WIN_OVER_WIDTH 15
#define WIN_OVER_Y 8
#define WIN_OVER_X 30



//generate unique ids for windows
//

enum window_id
{
	WIN_MENU,
	WIN_GAME,
	WIN_OVER
};


enum win_cat
{
	WT_MENU,
	WT_GAME,
	WT_OVER
};

struct invaderWindow
{
	enum window_id uid; //unique id
	enum win_cat w_cat;
	WINDOW * w_ptr; //pointer to window	
};
