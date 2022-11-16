#include "minesRLQVM.h"
#include <curses.h>

/*Cambia la pantalla que se este viendo (menus) o tablero*/
void change_screen(Tconfig config, int id);

/*actualiza toda la logica del juego*/
Tconfig update(Tconfig config);
/*actualiza la logica del menu*/
Tconfig update_menu(Tconfig config);
/*actualiza la logica del tablero*/
Tconfig update_board(Tconfig config);
/*actualiza la logica del menu de dificultad*/
Tconfig update_diffi(Tconfig config);

/*Dibuja lo que tenga que haber en pantalla*/
void draw(Tconfig config);
/*Dibuja el menu inicial*/
void draw_menu(Tconfig config);
/*Dibuja el tablero*/
void draw_board(Tconfig config);
/*Dibuja el menu de dificultad*/
void draw_diff(Tconfig config);

/*Controla las entradas/inputs botones*/
Tconfig inputs(Tconfig config);
/*Controla los botones en el menu inicial*/
Tconfig menu_inputs(Tconfig config);
/*Controla los botones en el tablero de juego*/
Tconfig board_inputs(Tconfig config);
/*Controla los botones en el menu de dificultad*/
Tconfig diff_inputs(Tconfig config);

/*Prepara el tablero para comenzar a jugar*/
Tconfig prepare_board(Tconfig config, int width, int height);
/*Genera las bombas y numeros*/
Tconfig gen_board(Tconfig config);
Tconfig clear_mist(Tconfig config);
Tconfig show_bombs(Tconfig config);

int main(){
	Tconfig config = {0, 0, '0'};
	int MAX_WIDTH, MAX_HEIGHT;
	srand(time(NULL));
	lncurses();
	getmaxyx(stdscr, MAX_HEIGHT, MAX_WIDTH);
	config = prepare_board(config, 19, 19);
	
	config.menu = create_container(0, 0, 31, 15, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);
	config.game_screen = create_container(0, 0, 33, 17, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);
	config.difficulty = create_container(0, 0, 31, 15, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);
	change_screen(config, 0);
	config.buttons[0] = create_button(0, -1,"Iniciar juego", 0, -1, 0);
	config.buttons[1] = create_button(0, 0, "Dificultad", 0, -1, 0);
	config.buttons[2] = create_button(0, 1, "Salir", 0, -1, 0);

	config.buttons[3] = create_button(0, -1,"Facil  9x9", 0, -1, 0);
	config.buttons[4] = create_button(0, 0, "Normal 13x13", 0, -1, 0);
	config.buttons[5] = create_button(0, 1, "Dificil 17x17", 0, -1, 0);
	config.buttons[6] = create_button(0, 2, "Volver", 0, -1, 0);
	
	game_loop(update, draw, inputs, config);
	endwin();
	return 0;
}

void change_screen(Tconfig config, int id){
	hide_panel(config.menu.pane);
	hide_panel(config.game_screen.pane);
	hide_panel(config.difficulty.pane);
	switch (id) {
		case 0:
			show_panel(config.menu.pane);
		break;
		case 1:
			show_panel(config.game_screen.pane);
		break;
		case 2:
			show_panel(config.difficulty.pane);
		break;
	}
}

Tconfig update(Tconfig config){
	switch (config.game_status) {
		case 0:
			config = update_menu(config);
		break;
		case 2:
			config = update_diffi(config);
		break;
	}
	return config;
}

Tconfig update_menu(Tconfig config){
	int i;
	if (config.button_id > 2){
		config.button_id = 0;
	}
	if (config.button_id < 0){
		config.button_id = 2;
	}
	for (i = 0; i < 3; i++){
		if (config.button_id == i){
			config.buttons[config.button_id].ac = 1;
			config.buttons[config.button_id].fg = 2;
		}else{
			config.buttons[i].ac = 0;
			config.buttons[i].fg = 0;
		}
	}
	return config;
}

Tconfig update_diffi(Tconfig config){
	int i;
	if (config.button_id > 6){
		config.button_id = 3;
	}
	if (config.button_id < 3){
		config.button_id = 6;
	}
	for (i = 3; i < 7; i++){
		if (config.button_id == i){
			config.buttons[config.button_id].ac = 1;
			config.buttons[config.button_id].fg = 2;
		}else{
			config.buttons[i].ac = 0;
			config.buttons[i].fg = 0;
		}
	}
	return config;
}

void draw(Tconfig config){
	switch (config.game_status) {
		case 0:
			draw_menu(config);
		break;
		case 1:
			draw_board(config);
		break;
		case 2:
			draw_diff(config);
		break;
	}
}

void draw_menu(Tconfig config){
	int i;
	draw_container(config.menu, C_MAGENTA, C_NONE, 0);
	for (i = 0; i < 3; i++){
		draw_button(config.menu, config.buttons[i], 1, 1);
	}
}

void draw_board(Tconfig config){
	int x, y;
	int xx, yy;
	int cur = 0;
	int num;
	draw_container(config.game_screen, C_MAGENTA, C_NONE, 0);
	mvwaddch(config.game_screen.win, (config.game_board.cursor_y*2)+1, (config.game_board.cursor_x*4)+1, '[');
	mvwaddch(config.game_screen.win, (config.game_board.cursor_y*2)+1, (config.game_board.cursor_x*4)+3, ']');
	for (y = 1; y <= config.game_board.height; y++){
		for (x = 1; x <= config.game_board.width; x++){
			xx = x-1;
			yy = y-1;
			num = config.game_board.field[y][x][0];
			if (num > 0){
				init_pair(num+1, num+1, -1);
				wattron(config.game_screen.win, A_BOLD | COLOR_PAIR(num+1));
				mvwprintw(config.game_screen.win, (yy*2)+1, (xx*4)+2, "%i", config.game_board.field[y][x][0]);
				wattroff(config.game_screen.win, A_BOLD | COLOR_PAIR(num+1));
			}
			if (config.game_board.field[y][x][0] == -1){
				init_pair(5, 5, -1);
				wattron(config.game_screen.win, COLOR_PAIR(5));
				mvwaddch(config.game_screen.win, (yy*2)+1, (xx*4)+2, '*');
				wattroff(config.game_screen.win, COLOR_PAIR(5));
			}
			if (config.game_board.field[y][x][1] == 1){
				mvwaddch(config.game_screen.win, (yy*2)+1, (xx*4)+2, '#');
			}
			if (config.game_board.field[y][x][1] == 3){
				init_pair(1, 1, -1);
				wattron(config.game_screen.win, COLOR_PAIR(1));
				mvwaddch(config.game_screen.win, (yy*2)+1, (xx*4)+2, 'V');
				wattroff(config.game_screen.win, COLOR_PAIR(1));
			}
		}
	}
}

void draw_diff(Tconfig config){
	int i;
	draw_container(config.difficulty, C_GREEN, C_NONE, 0);
	for (i = 3; i < 7; i++){
		draw_button(config.difficulty, config.buttons[i], 1, 1);
	}
}

Tconfig inputs(Tconfig config){
	switch (config.game_status) {
		case 0:
			config = menu_inputs(config);
		break;
		case 1:
			config = board_inputs(config);
		break;
		case 2:
			config = diff_inputs(config);
		break;
	}
	
	return config;
}

Tconfig menu_inputs(Tconfig config){
	int key = getch();
	if (key == 's' || key == KEY_DOWN){
		config.button_id+=1;
	}
	if (key == 'w' || key == KEY_UP){
		config.button_id-=1;
	}
	if (key == '\n'){
		switch (config.button_id) {
			case 2:
				key = 'q';
			break;
			case 0:
				config.game_status = 1;
				switch (config.game_board.dif) {
					case 0:
						config = prepare_board(config, 9, 9);
					break;
					case 1:
						config = prepare_board(config, 13, 13);
					break;
					case 2:
						config = prepare_board(config, 17, 17);
					break;
				}
				change_screen(config, config.game_status);
			break;
			case 1:
				config.game_status = 2;
				config.button_id = 3;
				change_screen(config, config.game_status);
			break;
		}
	}
	config.key = key;
	return config;
}

Tconfig board_inputs(Tconfig config){
	int key = getch();
	int x, y;
	if (key == 's' || key == KEY_DOWN){
		config.game_board.cursor_y+=1;
	}
	if (key == 'w' || key == KEY_UP){
		config.game_board.cursor_y-=1;
	}
	if (key == 'd' || key == KEY_RIGHT){
		config.game_board.cursor_x+=1;
	}
	if (key == 'a' || key == KEY_LEFT){
		config.game_board.cursor_x-=1;
	}
	if (config.game_board.cursor_x >= config.game_board.width){
		config.game_board.cursor_x = 0;
	}
	if (config.game_board.cursor_y >= config.game_board.height){
		config.game_board.cursor_y = 0;
	}
	if (config.game_board.cursor_x < 0){
		config.game_board.cursor_x = config.game_board.width-1;
	}
	if (config.game_board.cursor_y < 0){
		config.game_board.cursor_y = config.game_board.height-1;
	}
	x = config.game_board.cursor_x;
	y = config.game_board.cursor_y;
	if (key == '\n' || key == ' ')
	{
		config.game_board.field[y+1][x+1][1] = 0;
		if (config.game_board.status == 0){
			config = gen_board(config);
			config.game_board.status = 1;
		}
		if (config.game_board.field[y+1][x+1][0] == -1){
			config = show_bombs(config);
		}else{
			config = clear_mist(config);
		}
	}
	if (key == 'f'){
		if (config.game_board.field[y+1][x+1][1] == 1)
		{
			config.game_board.field[y+1][x+1][1] = 3;
		}
		else if (config.game_board.field[y+1][x+1][1] == 3)
		{
			config.game_board.field[y+1][x+1][1] = 1;
		}
	}
	if (key == 'q'){
		config.game_status = 0;
		config.button_id = 0;
		change_screen(config, config.game_status);
		key = '0';
	}
	config.key = key;
	return config;
}
Tconfig clear_mist(Tconfig config){
	int i, x, y, xx, yy, dir = 0, current_step = 1, max_steps = 1, turn_cont = 0;
	short place = 0;
	x = config.game_board.width/2;
	y = config.game_board.height/2;
	for (i = 0; i < (config.game_board.width*config.game_board.height)-1; i++){
		switch (dir) {
			case 0:
				x++;
			break;
			case 1:
				y++;
			break;
			case 2:
				x--;
			break;
			case 3:
				y--;
			break;
		}
		if (current_step >= max_steps){
			current_step = 0;
			turn_cont++;
			dir++;
		}
		if (turn_cont >= 2){
			turn_cont = 0;
			max_steps++;
		}
		if (dir > 3){
			dir = 0;
		}
		current_step++;
		xx = x+1;
		yy = y+1;
		if (config.game_board.field[yy][xx+1][1] == 0){
			place = 1;
		}
		if (config.game_board.field[yy][xx-1][1] == 0){
			place = 1;
		}
		if (config.game_board.field[yy+1][xx][1] == 0){
			place = 1;
		}
		if (config.game_board.field[yy-1][xx][1] == 0){
			place = 1;
		}
		if (config.game_board.field[yy][xx][1] == 2){
			place = 0;
		}
		if (config.game_board.field[yy][xx][1] == 3){
			place = 0;
		}
		if (config.game_board.field[yy][xx][0] > 0){
			place = 0;
		}
		if (config.game_board.field[yy][xx][0] == -1){
			place = 0;
		}
		if (config.game_board.field[yy][xx][0] == -2){
			place = 0;
		}
		if (place == 1){
			config.game_board.field[yy][xx][1] = 0;
		}
		place = 0;
	}
	for (y = 1; y <= config.game_board.height; y++) {
		for (x = 1; x <= config.game_board.width; x++) {
			if (config.game_board.field[y][x][0] > 0){
				if (config.game_board.field[y][x][1] != 3){
					if (config.game_board.field[y][x+1][1] == 0){
						config.game_board.field[y][x][1] = 2;
					}
					if (config.game_board.field[y][x-1][1] == 0){
						config.game_board.field[y][x][1] = 2;
					}
					if (config.game_board.field[y+1][x][1] == 0){
						config.game_board.field[y][x][1] = 2;
					}
					if (config.game_board.field[y-1][x][1] == 0){
						config.game_board.field[y][x][1] = 2;
					}
				}
			}
		}
	}
	return config;
}
Tconfig show_bombs(Tconfig config){
	int x, y;
	for (y = 1; y <= config.game_board.height; y++) {
		for (x = 1; x <= config.game_board.width; x++) {
			if (config.game_board.field[y][x][0] == -1){
				config.game_board.field[y][x][1] = 0;
			}
		}
	}
	return config;
}

Tconfig diff_inputs(Tconfig config){
	int key = getch();
	if (key == 's' || key == KEY_DOWN){
		config.button_id+=1;
	}
	if (key == 'w' || key == KEY_UP){
		config.button_id-=1;
	}
	if (key == '\n'){
		switch (config.button_id) {
			case 3:
				config.game_board.dif = 0;
			break;
			case 4:
				config.game_board.dif = 1;
			break;
			case 5:
				config.game_board.dif = 2;
			break;
		}
		config.game_status = 0;
		change_screen(config, 0);
		config.button_id = 0;
	}
	config.key = key;
	return config;
}

Tconfig prepare_board(Tconfig config, int width, int height){
	int y, x;
	config.game_board.status = 0;
	config.game_board.width = width;
	config.game_board.height = height;

	config.game_board.cursor_x = width/2;
	config.game_board.cursor_y = height/2;
	
	config.game_screen.height = (height*2)+1;
	config.game_screen.width = (config.game_screen.height*2)-1;
	for (y = 0; y < 19; y++){
		for (x = 0; x < 19; x++){
			config.game_board.field[y][x][0] = -2;
			config.game_board.field[y][x][1] = 1;
		}
	}
	return config;
}

Tconfig gen_board(Tconfig config)
{
	int y, x, mines, cont;
	switch (config.game_board.dif) {
		case 0:
			mines = 10;
		break;
		case 1:
			mines = 40;
		break;
		case 2:
			mines = 100;
		break;
	}
	while(mines > 0){
		if ((rand()%10) == 0)
		{
			y = (rand()%config.game_board.height-1)+1;
			x = (rand()%config.game_board.width-1)+1;
			if (config.game_board.field[y+1][x+1][0] != -11){
				if (x != config.game_board.cursor_x && y != config.game_board.cursor_y)
				{
					config.game_board.field[y+1][x+1][0] = -1;
					mines--;
				}
			}
		}
	}
	for (y = 1; y <= config.game_board.height; y++){
		for (x = 1; x <= config.game_board.width; x++){
			cont = 0;
			if (config.game_board.field[y-1][x-1][0] == -1){
				cont++;
			}
			if (config.game_board.field[y+1][x+1][0] == -1){
				cont++;
			}
			if (config.game_board.field[y-1][x+1][0] == -1){
				cont++;
			}
			if (config.game_board.field[y+1][x-1][0] == -1){
				cont++;
			}
			if (config.game_board.field[y-1][x][0] == -1){
				cont++;
			}
			if (config.game_board.field[y][x-1][0] == -1){
				cont++;
			}
			if (config.game_board.field[y+1][x][0] == -1){
				cont++;
			}
			if (config.game_board.field[y][x+1][0] == -1){
				cont++;
			}
			if (config.game_board.field[y][x][0] == -2){
				config.game_board.field[y][x][0] = cont;
			}
			cont = 0;
		}
	}
	return config;
}
