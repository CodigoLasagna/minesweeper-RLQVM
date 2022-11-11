#include <curses.h>
#include <panel.h>
#include <stdio.h>
#include <string.h>
#include "minesRLQVM.h"

int update();
int update_menu();

void draw();
void draw_menu();
void draw_game_screen();
int inputs();
int menu_inputs();

Tcontainer menu, game_screen;

Tbutton button[5];

int MAX_WIDTH, MAX_HEIGHT, BTN_ID = 0;
int key, game_status = 0;

int main(){
	lncurses();
	getmaxyx(stdscr, MAX_HEIGHT, MAX_WIDTH);
	
	menu = create_container(0, 0, 31, 15, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);
	game_screen = create_container(0, 0, 33, 17, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);
	hide_panel(game_screen.pane);
	button[0] = create_button(0, -1,"Iniciar juego", 1, -1, 0);
	button[1] = create_button(0, 0, "Dificultad", 1, -1, 0);
	button[2] = create_button(0, 1, "Salir", 1, -1, 0);
	
	game_loop(update, draw, inputs);
	endwin();
	return 0;
}

int update(){
	update_menu();
	return 1;
}

int update_menu(){
	int i;
	for (i = 0; i < 3; i++){
		if (BTN_ID == i){
			button[BTN_ID].ac = 1;
		}else{
			button[i].ac = 0;
		}
	}
	if (BTN_ID >= 3){
		BTN_ID = 0;
	}
	if (BTN_ID < 0){
		BTN_ID = 3;
	}
	return 0;
}

void draw(){
	switch (game_status) {
		case 0:
			draw_menu();
		break;
		case 1:
			draw_game_screen();
		break;
	}
}

void draw_menu(){
	int i;
	draw_container(menu, C_GREEN, C_NONE, 0);
	for (i = 0; i < 3; i++){
		draw_button(menu, button[i], 0, 1, 1, 1);
	}
}
void draw_game_screen(){
	int x, y;
	draw_container(game_screen, C_GREEN, C_NONE, 0);
	for (y = 0; y < 8; y++){
		for (x = 0; x < 8; x++){
			mvwaddch(game_screen.win, (y*2)+1, (x*4)+2, '#');
		}
	}
}

int inputs(){
	switch (game_status) {
		case 0:
			key = menu_inputs();
		break;
	}
	
	return key;
}

int menu_inputs(){
	key = getch();
	if (key == 's'){
		BTN_ID+=1;
	}
	if (key == 'w'){
		BTN_ID-=1;
	}
	if (key == '\n'){
		switch (BTN_ID) {
			case 2:
				key = 'q';
			break;
			case 0:
				game_status = 1;
				show_panel(game_screen.pane);
				hide_panel(menu.pane);
			break;
		}
	}
	return key;
}
