#include "minesRLQVM.h"

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
/*Dibuja la seccion de status*/
void draw_status(Tconfig config);
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
Tconfig prepare_board(Tconfig config, int width, int height, int mines);
/*Genera las bombas y numeros*/
Tconfig gen_board(Tconfig config);
Tconfig spiral_clear(Tconfig config);
Tconfig clear_mist(Tconfig config);
Tconfig show_bombs(Tconfig config);

Tconfig input_name(Tconfig config, int key);
Tconfig load_rank(Tconfig config);
int shellSort(Tind indices[], int n);

int main()
{
	Tconfig config = {0, 0, '0'};
	int MAX_WIDTH, MAX_HEIGHT;
	srand(10);
	lncurses();
	getmaxyx(stdscr, MAX_HEIGHT, MAX_WIDTH);
	config.name[0] = '_';
	config.name[1] = '_';
	config.name[2] = '_';
	config.name[3] = '\0';
	config.name_cid = 0;
	config = prepare_board(config, 19, 19, 0);
	config.game_board.altModes = 0;
	
	config.menu = create_container(0, 0, 31, 15, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);
	config.game_screen = create_container(0, 0, 34, 17, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);
	config.difficulty = create_container(0, 0, 31, 15, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);
	config.game_info = create_container(0, 12, 37, 6, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);
	config.name_input = create_container(0, 0, 9, 3, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);
	config.ranking = create_container(0, 0, 23, 17, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);
	change_screen(config, 0);
	config.buttons[0] = create_button(0, -1,"Iniciar juego", 0, -1, 0);
	config.buttons[1] = create_button(0, 0, "Dificultad", 0, -1, 0);
	config.buttons[2] = create_button(0, 1, "Salir", 0, -1, 0);
	
	config.buttons[3] = create_button(0, -2,"Facil  9x9 [10M]", 0, -1, 0);
	config.buttons[4] = create_button(0, -1, "Normal 13x13 [40M]", 0, -1, 0);
	config.buttons[5] = create_button(0, 0, "Dificil 17x17 [100M]", 0, -1, 0);
	config.buttons[6] = create_button(0, 2, "Vidas [ ]", 0, -1, 0);
	config.buttons[7] = create_button(0, 4, "Volver", 0, -1, 0);
	
	
	game_loop(update, draw, inputs, config);
	endwin();
	return 0;
}

void change_screen(Tconfig config, int id)
{
	hide_panel(config.menu.pane);
	hide_panel(config.game_screen.pane);
	hide_panel(config.difficulty.pane);
	hide_panel(config.game_info.pane);
	hide_panel(config.name_input.pane);
	hide_panel(config.ranking.pane);
	switch (id)
	{
		case 0:
			show_panel(config.menu.pane);
		break;
		case 1:
			show_panel(config.game_screen.pane);
			show_panel(config.game_info.pane);
		break;
		case 2:
			show_panel(config.difficulty.pane);
		break;
	}
}

Tconfig update(Tconfig config)
{
	switch (config.game_status)
	{
		case 0:
			config = update_menu(config);
		break;
		case 1:
			config = update_board(config);
		break;
		case 2:
			config = update_diffi(config);
		break;
	}
	return config;
}

Tconfig update_menu(Tconfig config)
{
	int i;
	if (config.button_id > 2)
	{
		config.button_id = 0;
	}
	if (config.button_id < 0)
	{
		config.button_id = 2;
	}
	for (i = 0; i < 3; i++)
	{
		if (config.button_id == i)
		{
			config.buttons[config.button_id].ac = 1;
			config.buttons[config.button_id].fg = C_GREEN;
		}
		else
		{
			config.buttons[i].ac = 0;
			config.buttons[i].fg = C_BLACK;
		}
	}
	return config;
}

Tconfig update_board(Tconfig config)
{
	int x, y, cells;
	if (config.game_board.status == 1)
	{
		config.game_board.timer = time(NULL) - config.game_board.delta;
	}
	if (config.game_board.cursor_x >= config.game_board.width)
	{
		config.game_board.cursor_x = 0;
	}
	if (config.game_board.cursor_y >= config.game_board.height)
	{
		config.game_board.cursor_y = 0;
	}
	if (config.game_board.cursor_x < 0)
	{
		config.game_board.cursor_x = config.game_board.width-1;
	}
	if (config.game_board.cursor_y < 0)
	{
		config.game_board.cursor_y = config.game_board.height-1;
	}
	x = config.game_board.cursor_x+1;
	y = config.game_board.cursor_y+1;
	if (config.game_board.update_map)
	{
		if (config.game_board.status == 0)
		{
			config = gen_board(config);
			config.game_board.status = 1;
			config.game_board.smileyStatus = 0;
		}
		if (config.game_board.status < 2)
		{
			if (config.game_board.field[y][x][1] != 0)
			{
				config.game_board.field[y][x][1] = 0;
				config.game_board.smileyStatus = 0;
			}
			
			if (config.game_board.field[y][x][0] > 0)
			{
				if (config.game_board.field[y][x][1] != 2)
				{
					config.game_board.field[y][x][1] = 2;
				}
				switch (config.game_board.field[y][x][0])
				{
					case 1:
					case 2:
						config.game_board.smileyStatus = 1;
					break;
					case 3:
					case 4:
						config.game_board.smileyStatus = 2;
					break;
					case 5:
					case 6:
						config.game_board.smileyStatus = 3;
					break;
					case 7:
					case 8:
						config.game_board.smileyStatus = 4;
					break;
				}
			}
			if (config.game_board.field[y][x][0] == -1)
			{
				config.game_board.hearts--;
				config.game_board.field[y][x][1] = 4;
				config.game_board.smileyStatus = 8;
				if (config.game_board.hearts == 0 && config.game_board.status != 2)
				{
					config.game_board.smileyStatus = 5;
					config = show_bombs(config);
					config.game_board.status = 2;
				}
			}
			else
			{
				config = clear_mist(config);
			}
		}
		config.game_board.update_map = 0;
	}
	cells = config.game_board.width*config.game_board.height;
	cells -= config.game_board.mines;
	for(y = 1; y < config.game_board.width+1; y++)
	{
		for(x = 1; x < config.game_board.width+1; x++)
		{
			if (config.game_board.field[y][x][1] != 1)
			{
				if (config.game_board.field[y][x][1] != 3)
				{
					if (config.game_board.field[y][x][0] != -1)
					{
						cells--;
					}
				}
			}
		}
	}
	config.game_board.freeCells = cells;
	if (cells == 0)
	{
		if (config.game_board.status < 3)
		{
			config.game_board.status = 3;
			config.game_board.smileyStatus = 7;
		}
	}
	return config;
}

Tconfig update_diffi(Tconfig config)
{
	int i;
	if (config.button_id > 7){
		config.button_id = 3;
	}
	if (config.button_id < 3){
		config.button_id = 7;
	}
	for (i = 3; i < 8; i++)
	{
		if (config.button_id == i)
		{
			config.buttons[config.button_id].ac = 1;
			config.buttons[config.button_id].fg = C_GREEN;
		}
		else
		{
			config.buttons[i].ac = 0;
			config.buttons[i].fg = C_BLACK;
		}
	}
	return config;
}

void draw(Tconfig config)
{
	switch (config.game_status)
	{
		case 0:
			draw_menu(config);
		break;
		case 1:
			draw_board(config);
			draw_status(config);
		break;
		case 2:
			draw_diff(config);
		break;
	}
}

void draw_menu(Tconfig config)
{
	int i;
	draw_container(config.menu, C_BLACK, C_NONE, 1);
	for (i = 0; i < 3; i++)
	{
		draw_button(config.menu, config.buttons[i], 1, 1);
	}
}

void draw_board(Tconfig config)
{
	int x, y, i, h;
	int xx, yy;
	int cur = 0;
	int num;
	FILE *archivo;
	Trank rank;
	draw_container(config.game_screen, C_BLACK, C_NONE, 1);
	draw_container(config.name_input, C_BLACK, C_NONE, 1);
	draw_container(config.ranking, C_CYAN, C_NONE, 1);
	mvwaddch(config.game_screen.win, (config.game_board.cursor_y*2)+1, (config.game_board.cursor_x*4)+1, '[');
	mvwaddch(config.game_screen.win, (config.game_board.cursor_y*2)+1, (config.game_board.cursor_x*4)+3, ']');
	mvwprintw(config.name_input.win, 1, 2, "%c %c %c", config.name[0], config.name[1], config.name[2]);
	for (y = 1; y <= config.game_board.height; y++)
	{
		for (x = 1; x <= config.game_board.width; x++)
		{
			xx = x-1;
			yy = y-1;
			num = config.game_board.field[y][x][0];
			if (num > 0)
			{
				wattron(config.game_screen.win, A_BOLD | COLOR_PAIR(num+2));
				mvwprintw(config.game_screen.win, (yy*2)+1, (xx*4)+2, "%i", config.game_board.field[y][x][0]);
				wattroff(config.game_screen.win, A_BOLD | COLOR_PAIR(num+2));
			}
			if (config.game_board.field[y][x][0] == -1)
			{
				wattron(config.game_screen.win, A_BOLD | COLOR_PAIR(C_RED));
				mvwaddch(config.game_screen.win, (yy*2)+1, (xx*4)+2, '*');
				wattroff(config.game_screen.win, A_BOLD | COLOR_PAIR(C_RED));
			}
			if (config.game_board.field[y][x][1] == 1)
			{
				wattron(config.game_screen.win, A_BOLD | COLOR_PAIR(C_BLACK));
				mvwaddch(config.game_screen.win, (yy*2)+1, (xx*4)+2, '#');
				wattroff(config.game_screen.win, A_BOLD | COLOR_PAIR(C_BLACK));
			}
			if (config.game_board.field[y][x][1] == 4)
			{
				wattron(config.game_screen.win, COLOR_PAIR(C_RED));
				mvwaddch(config.game_screen.win, (yy*2)+1, (xx*4)+2, 'O');
				wattroff(config.game_screen.win, COLOR_PAIR(C_RED));
			}
			if (config.game_board.field[y][x][1] == 3)
			{
				wattron(config.game_screen.win, COLOR_PAIR(C_RED));
				mvwaddch(config.game_screen.win, (yy*2)+1, (xx*4)+2, 'V');
				wattroff(config.game_screen.win, COLOR_PAIR(C_RED));
			}
		}
	}
	if (config.game_board.status == 2)
	{
		wattron(config.game_screen.win, COLOR_PAIR(C_RED));
		mvwprintw(config.game_screen.win, config.game_screen.height-1, (config.game_screen.width/2)-7, "[FIN DEL JUEGO]");
		wattroff(config.game_screen.win, COLOR_PAIR(C_RED));
	}
	if (config.game_board.status == 3)
	{
		wattron(config.game_screen.win, COLOR_PAIR(C_GREEN));
		mvwprintw(config.game_screen.win, config.game_screen.height-1, (config.game_screen.width/2)-8, "[CAMPO DESPEJADO]");
		wattroff(config.game_screen.win, COLOR_PAIR(C_GREEN));
	}
	if (config.game_board.status == 5)
	{
		i = 0;
		archivo = fopen("ranking.dat", "rb");
		mvwprintw(config.ranking.win, 0, 1, "[NOM TIEM        DIF]");
		if (config.game_board.dif+(config.game_board.altModes*3) > 2)
		{
			mvwprintw(config.ranking.win, 0, 11, "VIDAS");
		}
		if (archivo)
		{
			for (x = 0; x < config.n_ranks; x++)
			{
				fseek(archivo, config.ranks[x].pos*sizeof(Trank), SEEK_SET);
				fread(&rank, sizeof(Trank), 1, archivo);
				if (rank.type == config.game_board.dif+(config.game_board.altModes*3) && x < 15)
				{
					mvwprintw(config.ranking.win, i+1, 2, "%-4s %-12i %i", rank.name, rank.time, rank.diff+1);
					if (rank.type > 2)
					{
						for(h = 0; h < rank.hearts; h++)
						{
							wattron(config.ranking.win, COLOR_PAIR(C_RED));
							mvwprintw(config.ranking.win, i+1, 11+(h*2), "<3");
							wattroff(config.ranking.win, COLOR_PAIR(C_RED));
						}
					}
					i++;
				}
			}
			fclose(archivo);
		}
	}
}

void draw_status(Tconfig config)
{
	int i;
	draw_container(config.game_info, C_YELLOW, C_NONE, 1);
	wattron(config.game_info.win, COLOR_PAIR(C_YELLOW));
	mvwprintw(config.game_info.win, 2, (config.game_info.width)-10, "[T]: %i", config.game_board.timer);
	mvwprintw(config.game_info.win, 0, (config.game_screen.width/2)-9, "[ESTATUS DEL JUEGO]");
	mvwprintw(config.game_info.win, 2, 3, "[ ]: %i", config.game_board.flags);
	wattroff(config.game_info.win, COLOR_PAIR(C_YELLOW));
	wattron(config.game_info.win, COLOR_PAIR(C_RED));
	mvwprintw(config.game_info.win, 2, 4, "V");
	wattroff(config.game_info.win, COLOR_PAIR(C_RED));
	wattron(config.game_info.win, COLOR_PAIR(C_YELLOW));
	mvwprintw(config.game_info.win, 3, 3, "[ ]: %i", config.game_board.freeCells);
	wattroff(config.game_info.win, COLOR_PAIR(C_YELLOW));
	wattron(config.game_info.win, A_BOLD | COLOR_PAIR(C_BLACK));
	mvwprintw(config.game_info.win, 3, 4, "#");
	wattroff(config.game_info.win, A_BOLD | COLOR_PAIR(C_CYAN));
	/*Smiley*/
	switch (config.game_board.smileyStatus)
	{
		case 0:
			wattron(config.game_info.win, COLOR_PAIR(C_GREEN));
			mvwprintw(config.game_info.win, 2, (config.game_screen.width/2)-2, "['u']");
			wattroff(config.game_info.win, COLOR_PAIR(C_GREEN));
		break;
		case 1:
			wattron(config.game_info.win, COLOR_PAIR(C_YELLOW));
			mvwprintw(config.game_info.win, 2, (config.game_screen.width/2)-2, "['o']");
			wattroff(config.game_info.win, COLOR_PAIR(C_YELLOW));
		break;
		case 2:
			wattron(config.game_info.win, COLOR_PAIR(C_MAGENTA));
			mvwprintw(config.game_info.win, 2, (config.game_screen.width/2)-2, "['-']");
			wattroff(config.game_info.win, COLOR_PAIR(C_MAGENTA));
		break;
		case 3:
			wattron(config.game_info.win, COLOR_PAIR(C_BLUE));
			mvwprintw(config.game_info.win, 2, (config.game_screen.width/2)-2, "[;-;]");
			wattroff(config.game_info.win, COLOR_PAIR(C_BLUE));
		break;
		case 4:
			wattron(config.game_info.win, COLOR_PAIR(C_CYAN));
			mvwprintw(config.game_info.win, 2, (config.game_screen.width/2)-2, "[TnT]");
			wattroff(config.game_info.win, COLOR_PAIR(C_CYAN));
		break;
		case 5:
			wattron(config.game_info.win, COLOR_PAIR(C_RED));
			mvwprintw(config.game_info.win, 2, (config.game_screen.width/2)-2, "[x-x]");
			wattroff(config.game_info.win, COLOR_PAIR(C_RED));
		break;
		case 6:
			wattron(config.game_info.win, COLOR_PAIR(C_BLUE));
			mvwprintw(config.game_info.win, 2, (config.game_screen.width/2)-2, "[-^-]");
			wattroff(config.game_info.win, COLOR_PAIR(C_BLUE));
		break;
		case 7:
			wattron(config.game_info.win, A_BOLD | COLOR_PAIR(C_GREEN));
			mvwprintw(config.game_info.win, 2, (config.game_screen.width/2)-2, "[>v<]");
			wattroff(config.game_info.win, A_BOLD | COLOR_PAIR(C_GREEN));
		break;
		case 8:
			wattron(config.game_info.win, A_BOLD | COLOR_PAIR(C_MAGENTA));
			mvwprintw(config.game_info.win, 2, (config.game_screen.width/2)-2, "[>n<]");
			wattroff(config.game_info.win, A_BOLD | COLOR_PAIR(C_MAGENTA));
	}
	if (config.game_board.status >= 2)
	{
		wattron(config.game_info.win, COLOR_PAIR(C_YELLOW));
		mvwprintw(config.game_info.win, 4, (config.game_info.width/2)-16, "[Presione [ENTER] para continuar]");
		wattroff(config.game_info.win, COLOR_PAIR(C_YELLOW));
	}
	if (config.game_board.altModes == 1)
	{
		wattron(config.game_info.win, COLOR_PAIR(C_YELLOW));
		mvwprintw(config.game_info.win, 3, (config.game_info.width)-3, "]");
		mvwprintw(config.game_info.win, 3, (config.game_info.width)-12, "[");
		wattroff(config.game_info.win, COLOR_PAIR(C_YELLOW));
		wattron(config.game_info.win, COLOR_PAIR(C_RED));
		for (i = 0; i < config.game_board.hearts; i++)
		{
			mvwprintw(config.game_info.win, 3, (config.game_info.width)-(5+(i*3)), "<3");
		}
		wattroff(config.game_info.win, COLOR_PAIR(C_RED));
	}
	
}

void draw_diff(Tconfig config)
{
	int i;
	draw_container(config.difficulty, C_BLACK, C_NONE, 1);
	for (i = 3; i < 8; i++)
	{
		draw_button(config.difficulty, config.buttons[i], 1, 1);
	}
	if (config.game_board.altModes == 1)
	{
		wattron(config.difficulty.win, COLOR_PAIR(C_RED));
		mvwprintw(config.difficulty.win, (config.difficulty.height/2)+2, (config.difficulty.width/2)+3, "X");
		wattroff(config.difficulty.win, COLOR_PAIR(C_RED));
	}
}

Tconfig inputs(Tconfig config)
{
	switch (config.game_status)
	{
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

Tconfig menu_inputs(Tconfig config)
{
	int key = getch();
	if (key == 's' || key == KEY_DOWN || key == 'k')
	{
		config.button_id+=1;
	}
	if (key == 'w' || key == KEY_UP || key == 'l')
	{
		config.button_id-=1;
	}
	if (key == '\n' || key == ' ')
	{
		switch (config.button_id)
		{
			case 2:
				key = 'q';
			break;
			case 0:
				config.game_status = 1;
				switch (config.game_board.dif)
				{
					case 0:
						config = prepare_board(config, 9, 9, 10);
					break;
					case 1:
						config = prepare_board(config, 13, 13, 40);
					break;
					case 2:
						config = prepare_board(config, 17, 17, 100);
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
	if (config.game_board.status < 4)
	{
		if (key == 's' || key == KEY_DOWN || key == 'k')
		{
			config.game_board.cursor_y+=1;
		}
		if (key == 'w' || key == KEY_UP || key == 'l')
		{
			config.game_board.cursor_y-=1;
		}
		if (key == 'd' || key == KEY_RIGHT || key == ';')
		{
			config.game_board.cursor_x+=1;
		}
		if (key == 'a' || key == KEY_LEFT || key == 'j')
		{
			config.game_board.cursor_x-=1;
		}
	}
	x = config.game_board.cursor_x+1;
	y = config.game_board.cursor_y+1;
	if (key == 'f' && config.game_board.status == 1)
	{
		if (config.game_board.field[y][x][1] == 1)
		{
			if (config.game_board.flags > 0){
				config.game_board.field[y][x][1] = 3;
				config.game_board.flags--;
			}
			config.game_board.smileyStatus = 6;
		}
		else if (config.game_board.field[y][x][1] == 3)
		{
			config.game_board.field[y][x][1] = 1;
			config.game_board.flags++;
			config.game_board.smileyStatus = 2;
		}
	}
	if (key == '\n' || key == ' ')
	{
		if (config.game_board.field[y][x][1] == 1)
		{
			config.game_board.update_map = 1;
		}
		if (config.game_board.status == 2)
		{
			config.game_status = 0;
			config.button_id = 0;
			change_screen(config, config.game_status);
			key = '0';
		}
		if (config.game_board.status == 3)
		{
			show_panel(config.name_input.pane);
			config.game_board.status = 4;
		}
	}
	if (key == 'q')
	{
		config.game_status = 0;
		config.button_id = 0;
		change_screen(config, config.game_status);
		key = '0';
	}
	if (config.game_board.status == 5)
	{
		config.game_status = 0;
		change_screen(config, config.game_status);
	}
	if (config.game_board.status == 4)
	{
		config = input_name(config, key);
	}
	config.key = key;
	return config;
}

Tconfig spiral_clear(Tconfig config)
{
	int i, x, y, dir = 0, current_step = 1, max_steps = 1, turn_cont = 0;
	short place = 0;
	x = config.game_board.width/2;
	y = config.game_board.height/2;
	x++;
	y++;
	for (i = 0; i < (config.game_board.width*config.game_board.height); i++)
	{
		if (config.game_board.field[y][x+1][1] == 0){
			place = 1;
		}
		if (config.game_board.field[y][x-1][1] == 0){
			place = 1;
		}
		if (config.game_board.field[y+1][x][1] == 0){
			place = 1;
		}
		if (config.game_board.field[y-1][x][1] == 0){
			place = 1;
		}

		if (config.game_board.field[y+1][x+1][1] == 0){
			place = 1;
		}
		if (config.game_board.field[y-1][x-1][1] == 0){
			place = 1;
		}
		if (config.game_board.field[y+1][x-1][1] == 0){
			place = 1;
		}
		if (config.game_board.field[y-1][x+1][1] == 0){
			place = 1;
		}

		if (config.game_board.field[y][x][0] > 0){
			place = 0;
		}
		if (config.game_board.field[y][x][1] == 2){
			place = 0;
		}
		if (config.game_board.field[y][x][0] == -1){
			place = 0;
		}
		if (config.game_board.field[y][x][1] == 3){
			place = 0;
		}
		if (place == 1)
		{
			if (config.game_board.field[y][x][1] == 1)
			{
				config.game_board.field[y][x][1] = 0;
			}
		}
		place = 0;
		switch (dir)
		{
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
		if (current_step >= max_steps)
		{
			current_step = 0;
			turn_cont++;
			dir++;
		}
		if (turn_cont >= 2)
		{
			turn_cont = 0;
			max_steps++;
		}
		if (dir > 3)
		{
			dir = 0;
		}
		current_step++;
	}
	return config;
}

Tconfig clear_mist(Tconfig config)
{
	int i, x, y, place = 0;
	for (i = 0; i <= 10; i++)
	{
		config = spiral_clear(config);
	}
	for (y = 1; y <= config.game_board.height; y++)
	{
		for (x = 1; x <= config.game_board.width; x++) 
		{
			if (config.game_board.field[y][x][0] > 0)
			{
				if (config.game_board.field[y][x][1] != 3)
				{
					if (config.game_board.field[y][x][1] != 2)
					{
						if (config.game_board.field[y][x+1][1] == 0)
						{
							place = 1;
						}
						if (config.game_board.field[y][x-1][1] == 0)
						{
							place = 1;
						}
						if (config.game_board.field[y+1][x][1] == 0)
						{
							place = 1;
						}
						if (config.game_board.field[y-1][x][1] == 0)
						{
							place = 1;
						}
						if (config.game_board.field[y+1][x+1][1] == 0)
						{
							place = 1;
						}
						if (config.game_board.field[y-1][x-1][1] == 0)
						{
							place = 1;
						}
						if (config.game_board.field[y+1][x-1][1] == 0)
						{
							place = 1;
						}
						if (config.game_board.field[y-1][x+1][1] == 0)
						{
							place = 1;
						}
					}
				}
			}
			if (place == 1)
			{
				if (config.game_board.field[y][x][1] != 2)
				{
					config.game_board.field[y][x][1] = 2;
				}
			}
		place = 0;
		}
	}
	return config;
}

Tconfig show_bombs(Tconfig config)
{
	int x, y;
	for (y = 1; y <= config.game_board.height; y++)
	{
		for (x = 1; x <= config.game_board.width; x++)
		{
			if (config.game_board.field[y][x][0] == -1)
			{
				if (config.game_board.field[y][x][1] == 1)
				{
					config.game_board.field[y][x][1] = 0;
				}
				if (config.game_board.field[y][x][1] == 3)
				{
					config.game_board.field[y][x][1] = 4;
				}
			}
		}
	}
	return config;
}

Tconfig diff_inputs(Tconfig config)
{
	int key = getch();
	if (key == 's' || key == KEY_DOWN || key == 'k')
	{
		config.button_id+=1;
	}
	if (key == 'w' || key == KEY_UP || key == 'l')
	{
		config.button_id-=1;
	}
	if (key == '\n' || key == ' ')
	{
		switch (config.button_id)
		{
			case 3:
				config.game_board.dif = 0;
			break;
			case 4:
				config.game_board.dif = 1;
			break;
			case 5:
				config.game_board.dif = 2;
			break;
			case 6:
				config.game_board.altModes = !config.game_board.altModes;
			break;
		}
		if (config.button_id != 6)
		{
			config.game_status = 0;
			change_screen(config, 0);
			config.button_id = 0;
		}
	}
	config.key = key;
	return config;
}

Tconfig prepare_board(Tconfig config, int width, int height, int mines)
{
	int y, x;
	if (config.game_board.altModes == 1)
	{
		config.game_board.hearts = 3;
	}
	else
	{
		config.game_board.hearts = 1;
	}
	config.game_board.delta = time(NULL);
	config.game_board.smileyStatus = 2;
	config.game_board.status = 0;
	config.game_board.update_map = 0;
	config.game_board.width = width;
	config.game_board.height = height;
	
	config.game_board.cursor_x = width/2;
	config.game_board.cursor_y = height/2;
	
	config.game_screen.height = (height*2)+1;
	config.game_screen.width = (config.game_screen.height*2)-1;
	for (y = 0; y < 19; y++)
	{
		for (x = 0; x < 19; x++)
		{
			config.game_board.field[y][x][0] = -2;
			config.game_board.field[y][x][1] = 1;
		}
	}
	
	config.game_info.width = (config.game_screen.height*2)-1;
	config.game_info.y = -(height+3);
	config.game_board.mines = mines;
	config.game_board.flags = mines;
	config.game_board.freeCells = (width*height)-mines;
	
	return config;
}

Tconfig gen_board(Tconfig config)
{
	int y, x, mines, cont;
	mines = config.game_board.mines;
	while(mines > 0)
	{
		if ((rand()%10) == 0)
		{
			y = (rand()%config.game_board.height);
			x = (rand()%config.game_board.width);
			if (config.game_board.field[y+1][x+1][0] != -1)
			{
				if (x != config.game_board.cursor_x && y != config.game_board.cursor_y)
				{
					if (x != config.game_board.cursor_x+1 && y != config.game_board.cursor_y)
					{
						if (x != config.game_board.cursor_x-1 && y != config.game_board.cursor_y)
						{
							config.game_board.field[y+1][x+1][0] = -1;
							mines--;
						}
					}
				}
			}
		}
	}
	
	for (y = 1; y <= config.game_board.height; y++)
	{
		for (x = 1; x <= config.game_board.width; x++)
		{
			cont = 0;
			if (config.game_board.field[y-1][x-1][0] == -1)
			{
				cont++;
			}
			if (config.game_board.field[y+1][x+1][0] == -1)
			{
				cont++;
			}
			if (config.game_board.field[y-1][x+1][0] == -1)
			{
				cont++;
			}
			if (config.game_board.field[y+1][x-1][0] == -1)
			{
				cont++;
			}
			if (config.game_board.field[y-1][x][0] == -1)
			{
				cont++;
			}
			if (config.game_board.field[y][x-1][0] == -1)
			{
				cont++;
			}
			if (config.game_board.field[y+1][x][0] == -1)
			{
				cont++;
			}
			if (config.game_board.field[y][x+1][0] == -1)
			{
				cont++;
			}
			if (config.game_board.field[y][x][0] == -2)
			{
				config.game_board.field[y][x][0] = cont;
			}
			cont = 0;
		}
	}
	return config;
}

Tconfig input_name(Tconfig config, int key)
{
	FILE *archivo;
	Trank ranking;
	if (key > 64 && key < 91 || key > 96 && key < 123)
	{
		if (config.name_cid < 3)
		{
			if (key > 96)
			{
				key -= 32;
			}
			config.name[config.name_cid] = key;
			config.name_cid++;
		}
	}
	if (key == KEY_BACKSPACE && config.name_cid > 0)
	{
		config.name_cid--;
		config.name[config.name_cid] = '_';
	}
	if ((key == '\n' || key == ' ') && config.name_cid == 3)
	{
		if (config.game_board.status == 4)
		{
			hide_panel(config.name_input.pane);
			show_panel(config.ranking.pane);
			config.game_board.status = 5;
			archivo = fopen("ranking.dat", "a+b");
			if (archivo)
			{
				ranking.type = config.game_board.dif+(config.game_board.altModes*3);
				ranking.name[0] = config.name[0];
				ranking.name[1] = config.name[1];
				ranking.name[2] = config.name[2];
				ranking.name[3] = config.name[3];
				ranking.diff = config.game_board.dif;
				ranking.time = config.game_board.timer;
				ranking.hearts = config.game_board.hearts;
				fwrite(&ranking, sizeof(ranking), 1, archivo);
				fclose(archivo);
			}
			config = load_rank(config);
		}
	}
	return config;
}

Tconfig load_rank(Tconfig config)
{
	FILE *archivo;
	Trank rank;
	int i = 0;
	archivo = fopen("ranking.dat", "rb");
	if (archivo)
	{
		while (fread(&rank, sizeof(Trank), 1, archivo))
		{
			config.ranks[i].time = rank.time;
			config.ranks[i].pos = i;
			i++;
		}
		config.n_ranks = i;
		fclose(archivo);
		shellSort(config.ranks, i);
	}
	return config;
}

int shellSort(Tind indices[], int n) {
	int interval, i = 0, j = 0;
	Tind temp;
	for (interval = n / 2; interval > 0; interval /= 2) {
		for (i = interval; i < n; i += 1) {
			temp = indices[i];
			for (j = i; j >= interval && indices[j - interval].time > temp.time; j -= interval) {
			  indices[j] = indices[j - interval];
			}
			indices[j] = temp;
		}
	}
	return 0;
}
