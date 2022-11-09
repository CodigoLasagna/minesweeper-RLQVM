#include <stdio.h>
#include "minesRLQVM.h"

int update();
void draw();
	TWindow mainscreen;

int main(){
	lncurses();
	
	mainscreen = create_window(0, 0, 20, 10, 0, 0, 0, 0, 64, 32);
	game_loop(update, draw);
	getch();
	endwin();
	return 0;
}

int update(){
	return 1;
}
void draw(){
	colorizeWin(mainscreen, C_GREEN, C_NONE, 0);
}
