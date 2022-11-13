#include "minesRLQVM.h"

int update(Tconfig* config);
int update_menu(Tconfig* config);

void draw(Tconfig config);
void draw_menu();
void draw_game_screen();

int inputs(Tconfig* config);
int menu_inputs(Tconfig* config);

Tcontainer menu, game_screen;
Tbutton button[3];

int main(){
	Tconfig config = {0, 0};
	int MAX_WIDTH, MAX_HEIGHT;
	lncurses();
	getmaxyx(stdscr, MAX_HEIGHT, MAX_WIDTH);
	
	menu = create_container(0, 0, 31, 15, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);
	game_screen = create_container(0, 0, 33, 17, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);
	hide_panel(game_screen.pane);
	button[0] = create_button(0, -1,"Iniciar juego", 0, -1, 0);
	button[1] = create_button(0, 0, "Dificultad", 0, -1, 0);
	button[2] = create_button(0, 1, "Salir", 0, -1, 0);
	
	game_loop(update, draw, inputs, config);
	endwin();
	return 0;
}

int update(Tconfig* config){
	update_menu(config);
	return 1;
}

int update_menu(Tconfig* config){
	int i;
	if (config->button_id > 2){
		config->button_id = 0;
	}
	if (config->button_id < 0){
		config->button_id = 2;
	}
	for (i = 0; i < 3; i++){
		if (config->button_id == i){
			button[config->button_id].ac = 1;
			button[config->button_id].fg = 2;
		}else{
			button[i].ac = 0;
			button[i].fg = 0;
		}
	}
	return 0;
}

void draw(Tconfig config){
	switch (config.game_status) {
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
		draw_button(menu, button[i], 1, 1);
	}
}
void draw_game_screen(){
	int x, y;
	int cur = 0;
	draw_container(game_screen, C_GREEN, C_NONE, 0);
	for (y = 0; y < 8; y++){
		for (x = 0; x < 8; x++){
			mvwaddch(game_screen.win, (y*2)+1, (x*4)+2, '#');
		}
	}
}

int inputs(Tconfig* config){
	int key = 0;
	switch (config->game_status) {
		case 0:
			key = menu_inputs(config);
		break;
	}
	
	return key;
}

int menu_inputs(Tconfig* config){
	int key = getch();
	if (key == 's' || key == KEY_DOWN){
		config->button_id+=1;
	}
	if (key == 'w' || key == KEY_UP){
		config->button_id-=1;
	}
	if (key == '\n'){
		switch (config->button_id) {
			case 2:
				key = 'q';
			break;
			case 0:
				config->game_status = 1;
				show_panel(game_screen.pane);
				hide_panel(menu.pane);
			break;
		}
	}
	return key;
}
