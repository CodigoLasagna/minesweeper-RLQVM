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
/*algoritmo que despeja el mapa con cada descubrimiento de celda/casilla*/
Tconfig spiral_clear(Tconfig config);
/*se encarga de despejar la niebla de casillas reveladas*/
Tconfig clear_mist(Tconfig config);
/*Muestra todas las minas en caso de perder en cualquiera de los dos modos de juego*/
Tconfig show_bombs(Tconfig config);

/*función para introducir apodo/nickname al ranking*/
Tconfig input_name(Tconfig config, int key);
/*función para mostrar el ranking de cada modo y dificultad*/
Tconfig load_rank(Tconfig config);
/*shell sort para ordenar el ranking*/
int shellSort(Tind indices[], int n);

int main()
{
	Tconfig config = {0, 0, '0'};				/*se prepara una con el game_status, id de boton seleccionado y el input*/
	int MAX_WIDTH, MAX_HEIGHT;					/*variables para obtener el tamaño maximo de la terminal*/
	srand(time(NULL));							/*se genera una semilla aleatoria para los niveles*/
	lncurses();									/*se prepara el backend de la libreria ncurses*/
	getmaxyx(stdscr, MAX_HEIGHT, MAX_WIDTH);	/*se obtiene el tamaño de la terminal*/
	config.name[0] = '_';						/* se inicializa el nickname base para ael rank*/
	config.name[1] = '_';
	config.name[2] = '_';
	config.name[3] = '\0';
	config.name_cid = 0;						/*marca la posición del cursor en el nombre/nickname para el rank*/
	config = prepare_board(config, 19, 19, 0);	/*crea una configuración base para el tablero/mapa*/
	config.game_board.altModes = 0;				/*inicia el tablero en modo normal*/
	
	config.menu = create_container(0, 0, 31, 15, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);		/*se crea un contenedor que sera el menu*/
	config.game_screen = create_container(0, 0, 34, 17, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);	/*se crea un contenedor que sera el tablero*/
	config.difficulty = create_container(0, 0, 31, 15, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);	/*se crea un contenedor que sera el menu de dificultad*/
	config.game_info = create_container(0, 12, 37, 6, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);	/*se crea un contenedor que tendra la info de la partida*/
	config.name_input = create_container(0, 0, 9, 3, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);	/*se crea un contenedor para recibir el nickname una vez se gana una partida*/
	config.ranking = create_container(0, 0, 23, 17, 0, 0, 0, 1, MAX_WIDTH, MAX_HEIGHT);		/*se crea un contenedor para mostrar el ranking*/
	change_screen(config, 0);																/*se configura el juego para mostrar el menu inicial*/
	config.buttons[0] = create_button(0, -1,"Iniciar juego", 0, -1, 0);			/*se crea el boton de iniciar juego*/
	config.buttons[1] = create_button(0, 0, "Dificultad", 0, -1, 0);			/*se crea el boton para escoger la dificultad*/
	config.buttons[2] = create_button(0, 1, "Salir", 0, -1, 0);					/*se crea el boton para salir del juego*/
	
	config.buttons[3] = create_button(0, -2,"Facil  9x9 [10M]", 0, -1, 0);		/*se crea el boton para escoger dificultad fácil con 10 minas y tablero de 9x9 casillas*/
	config.buttons[4] = create_button(0, -1, "Normal 13x13 [40M]", 0, -1, 0);	/*se crea el boton para escoger dificultad normal con 40 minas y tablero de 13x13 casillas*/
	config.buttons[5] = create_button(0, 0, "Dificil 17x17 [100M]", 0, -1, 0);	/*se crea el boton para escoger dificultad dificil con 100 minas y tablero de 17x17 casillas*/
	config.buttons[6] = create_button(0, 2, "Vidas [ ]", 0, -1, 0);				/*se crea el boton para alternar entre modo con o sin vidas*/
	config.buttons[7] = create_button(0, 4, "Volver", 0, -1, 0);				/*se crea el boton para regresar al menu anterior*/
	
	game_loop(update, draw, inputs, config);	/*se inicia el game loop del juego o ciclo del juego*/
	endwin();									/*en caso de terminar el juego se cierra la libreria de ncurses*/
	return 0;									/*metodo de salida 0*/
}

/*funcion para seleccionar escena del juego, menu o contenedor
Recibe la estructura inicial con todas las configuracinoes y el ID de que menu deberia mostrarse*/
void change_screen(Tconfig config, int id)
{
	/*se esconden todos los menus o contenedores que no deben verse*/
	hide_panel(config.menu.pane);
	hide_panel(config.game_screen.pane);
	hide_panel(config.difficulty.pane);
	hide_panel(config.game_info.pane);
	hide_panel(config.name_input.pane);
	hide_panel(config.ranking.pane);
	/*en base al id se muestra el contenedor/es correcto/s*/
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

/*funcion para actualizar toda la logica del juego*/
Tconfig update(Tconfig config)
{
	switch (config.game_status)
	{
		case 0:
			config = update_menu(config);/*se actualiza la logica del menu inicial*/
		break;
		case 1:
			config = update_board(config);/*se actualiza la logica del tablero de juego o nivel*/
		break;
		case 2:
			config = update_diffi(config);/*se actualiza el menu de dificultad*/
		break;
	}
	return config;
}

/*funcion para actualizar el menu inicial del juego*/
Tconfig update_menu(Tconfig config)
{
	int i;
	/*seccion para calcular si estamos seleccionando un boton visible*/
	if (config.button_id > 2) /*si es mayor que 2 se regresa al boton inicial*/
	{
		config.button_id = 0;
	}
	if (config.button_id < 0)/*si es menor que 0 se va al boton final*/
	{
		config.button_id = 2;
	}
	/*ciclo for para actualizar el como se ven los botones*/
	for (i = 0; i < 3; i++)
	{
		if (config.button_id == i)/*si el id seleccionado coincide con un boton existente se muestra en su modo alterno*/
		{
			config.buttons[config.button_id].ac = 1;
			config.buttons[config.button_id].fg = C_GREEN;
		}
		else
		{
			/*los botones que no coinciden se muestran normal*/
			config.buttons[i].ac = 0;
			config.buttons[i].fg = C_BLACK;
		}
	}
	/*se retorna la configuracion*/
	return config;
}

/*funcion para actualizar el tablero de juego*/
Tconfig update_board(Tconfig config)
	{
	int x, y, cells;
	if (config.game_board.status == 1)/*mientras el tablero este en estatus 0 (ya que se haya hecho el primer movimiento) se cuenta el tiempo transcurrido de partida*/
	{
		config.game_board.timer = time(NULL) - config.game_board.delta;/*se saca una diferencia de tiempo para obtener el triempo transcurrido*/
	}
	/*seccion para mantener el cursor dentro del tablero*/
	if (config.game_board.cursor_x >= config.game_board.width)
	{
		config.game_board.cursor_x = 0;/*si el cursor sale completamente por la derecha se regresa al inicio(izquierda)*/
	}
	if (config.game_board.cursor_y >= config.game_board.height)
	{
		config.game_board.cursor_y = 0;/*si el cursor sale completamente por debajo se regresa al inicio(arriba)*/
	}
	if (config.game_board.cursor_x < 0)
	{
		config.game_board.cursor_x = config.game_board.width-1; /*si el cursor sale por la izquierda completamente se envia al final(derecha)*/
	}
	if (config.game_board.cursor_y < 0)
	{
		config.game_board.cursor_y = config.game_board.height-1; /*si el cursor sale completamente por arriba se envia al final (abajo)*/
	}
	x = config.game_board.cursor_x+1;/*se agrega uno mas a la posición del cursor para que coincida con el tablero visible y el tablero logico*/
	y = config.game_board.cursor_y+1;
	if (config.game_board.update_map)/* en caso de que el jugador haya realizado una accion de despeje*/
	{
		if (config.game_board.status == 0)/*si el estatus es 0 (aún no se ha realizado el primer despeje)*/
		{
			/*se genera el mapa, se cambia el estatus del juego a 1 (partida en curso) y se cambia el gesto de smileu*/
			config = gen_board(config);
			config.game_board.status = 1;
			config.game_board.smileyStatus = 0;
		}
		if (config.game_board.status < 2) /*si el estatus del juego  es sin iniciar o en curso*/
		{
			if (config.game_board.field[y][x][1] != 0)/*si la casilla aún no se ha despejado, se despeja*/
			{
				config.game_board.field[y][x][1] = 0;
				config.game_board.smileyStatus = 0;
			}
			
			if (config.game_board.field[y][x][0] > 0)/*si la casilla tiene un numero de minas mayor a 0 a la redonda*/
			{
				if (config.game_board.field[y][x][1] != 2)
				{
					config.game_board.field[y][x][1] = 2;/*se despeja la casilla con marca de 2 para que los calculos sean correctos*/
				}
				switch (config.game_board.field[y][x][0])/*dependiendo del numero de minas, smiley tendra un gesto distinto al realizar una acción*/
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
			if (config.game_board.field[y][x][0] == -1)/*en caso de que la casilla tenga una mina*/
			{
				config.game_board.hearts--;/*se quita uno de vida, si las vidas llegan a cero pierde partida*/
				config.game_board.field[y][x][1] = 4;/*la casilla se marca como mina encontrada*/
				config.game_board.smileyStatus = 8;/*smiley se pone mal*/
				if (config.game_board.hearts == 0 && config.game_board.status != 2)/* en caso de no tener vidas smiley muere y se muestran todas las minas*/
				{
					config.game_board.smileyStatus = 5;
					config = show_bombs(config);
					config.game_board.status = 2;/*se cambia el estatus del juego a partida perdida (estatus 2)*/
				}
			}
			else
			{
				config = clear_mist(config);/*en caso de que la casilla no tenga mina simplemente se despeja la casilla y se realiza una limpia de niebla*/
			}
		}
		config.game_board.update_map = 0;/*ya no se realizaran actualizaciones al mapa en las casillas totalmente*/
	}
	cells = config.game_board.width*config.game_board.height;/*se calculan las casillas por despejar*/
	cells -= config.game_board.mines;
	/*se revisan todas las casillas en el tablero*/
	for(y = 1; y < config.game_board.width+1; y++)
	{
		for(x = 1; x < config.game_board.width+1; x++)
		{
			if (config.game_board.field[y][x][1] != 1)/*primero se comprueba que sea una casilla despejada*/
			{
				if (config.game_board.field[y][x][1] != 3) /*luego se comprueba que sea una casilla sin bandera*/
				{
					if (config.game_board.field[y][x][0] != -1)  /* por ultimo se copmrueba que no haya una mina*/
					{
						cells--; /*por cada casilla libre que no tenga bomba se reduce el contador de casillas por despejar*/
					}
				}
			}
		}
	}
	config.game_board.freeCells = cells;/*se envia la cantidad de casillas al contador que se dibujara posteriormente*/
	if (cells == 0)
	{
		if (config.game_board.status < 3)/*si el estatus es menor que 3 es decir no victoria, se cambia a victoria y smiley hace un gesto feliz*/
		{
			config.game_board.status = 3;
			config.game_board.smileyStatus = 7;
		}
	}
	return config;/*se retorna la configuración*/
}

/*configuracion para acualizar el menu de dificultad*/
Tconfig update_diffi(Tconfig config)
{
	int i;
	/*lo mismo que con la seccion del menu inicial, se comprueba que el indicador este sobre uno de los botones visibles*/
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
			config.buttons[config.button_id].ac = 1;/*si el id coincide con el boton se cambia a imagen alternativa*/
			config.buttons[config.button_id].fg = C_GREEN;
		}
		else
		{
			config.buttons[i].ac = 0;/*los demas se cambian a imagen normal*/
			config.buttons[i].fg = C_BLACK;
		}
	}
	return config;/*se retorna la configuracion*/
}

/*funcion que se encarga de dibujar todo lo gráfico del juego*/
void draw(Tconfig config)
{
	/*dependiendo del estatus de todo el juego se dibuja el escenario correcto*/
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

/*se dibujan todos los elementos del menu inicial*/
void draw_menu(Tconfig config)
{
	int i;
	draw_container(config.menu, C_BLACK, C_NONE, 1);/*se dibuja el contenedor del menu*/
	for (i = 0; i < 3; i++)
	{
		draw_button(config.menu, config.buttons[i], 1, 1);/*se dibujan los botones del menu principal*/
	}
}

/*funcion que dibuja todo el tablero o mapa del juego*/
void draw_board(Tconfig config)
{
	int x, y, i, h;
	int xx, yy;
	int cur = 0;
	int num;
	FILE *archivo;
	Trank rank;
	draw_container(config.game_screen, C_BLACK, C_NONE, 1);/*se dibuja el tablero o mapa del juego*/
	draw_container(config.name_input, C_BLACK, C_NONE, 1);/*se dibuja el contenedor para el usuario una vez que se gane una partida*/
	draw_container(config.ranking, C_CYAN, C_NONE, 1);/*se dibuja el ranking una vez ganada una partida*/
	mvwaddch(config.game_screen.win, (config.game_board.cursor_y*2)+1, (config.game_board.cursor_x*4)+1, '[');/*se dibuja el lado izquierdo del cursor*/
	mvwaddch(config.game_screen.win, (config.game_board.cursor_y*2)+1, (config.game_board.cursor_x*4)+3, ']');/*se dibuja el lado derecho del cursor*/
	mvwprintw(config.name_input.win, 1, 2, "%c %c %c", config.name[0], config.name[1], config.name[2]);/*se dibujan los espacios del nombre una vez se gane una partida*/
	/*seccion para dibujar todo el tablero*/
	for (y = 1; y <= config.game_board.height; y++)
	{
		for (x = 1; x <= config.game_board.width; x++)
		{
			xx = x-1;/*se ajustan las coordenadas de donde dibujar cada cosa*/
			yy = y-1;
			num = config.game_board.field[y][x][0];/*se obtiene la cantidad de minas a la redonda de cada celda o casilla*/
			if (num > 0)/*si el contador de minas es mayor a 0 se dibuja un numero con su respectivo color*/
			{
				wattron(config.game_screen.win, A_BOLD | COLOR_PAIR(num+2));
				mvwprintw(config.game_screen.win, (yy*2)+1, (xx*4)+2, "%i", config.game_board.field[y][x][0]);
				wattroff(config.game_screen.win, A_BOLD | COLOR_PAIR(num+2));
			}
			if (config.game_board.field[y][x][0] == -1)/*si hay mina se dibuja con un * rojo*/
			{
				wattron(config.game_screen.win, A_BOLD | COLOR_PAIR(C_RED));
				mvwaddch(config.game_screen.win, (yy*2)+1, (xx*4)+2, '*');
				wattroff(config.game_screen.win, A_BOLD | COLOR_PAIR(C_RED));
			}
			if (config.game_board.field[y][x][1] == 1)/*si hay casilla se dibuja la casilla con un # oscuro encima de las minas, en caso de esta no estar despejada*/
			{
				wattron(config.game_screen.win, A_BOLD | COLOR_PAIR(C_BLACK));
				mvwaddch(config.game_screen.win, (yy*2)+1, (xx*4)+2, '#');
				wattroff(config.game_screen.win, A_BOLD | COLOR_PAIR(C_BLACK));
			}
			if (config.game_board.field[y][x][1] == 4)/*si la mina es descubierta o se muestra debajo de una bandera se dibuja con O*/
			{
				wattron(config.game_screen.win, COLOR_PAIR(C_RED));
				mvwaddch(config.game_screen.win, (yy*2)+1, (xx*4)+2, 'O');
				wattroff(config.game_screen.win, COLOR_PAIR(C_RED));
			}
			if (config.game_board.field[y][x][1] == 3)/*icono para dibujar la bandera*/
			{
				wattron(config.game_screen.win, COLOR_PAIR(C_RED));
				mvwaddch(config.game_screen.win, (yy*2)+1, (xx*4)+2, 'V');
				wattroff(config.game_screen.win, COLOR_PAIR(C_RED));
			}
		}
	}
	if (config.game_board.status == 2)/*en caso de perder partida se dibuja este mensaje en la parte inferior*/
	{
		wattron(config.game_screen.win, COLOR_PAIR(C_RED));
		mvwprintw(config.game_screen.win, config.game_screen.height-1, (config.game_screen.width/2)-7, "[FIN DEL JUEGO]");
		wattroff(config.game_screen.win, COLOR_PAIR(C_RED));
	}
	if (config.game_board.status == 3)/*en caso de ganar la partida se dibuja este mensaje en la parte inferior*/
	{
		wattron(config.game_screen.win, COLOR_PAIR(C_GREEN));
		mvwprintw(config.game_screen.win, config.game_screen.height-1, (config.game_screen.width/2)-8, "[CAMPO DESPEJADO]");
		wattroff(config.game_screen.win, COLOR_PAIR(C_GREEN));
	}
	if (config.game_board.status == 5)/*seccion para dibujar el ranking*/
	{
		i = 0;
		archivo = fopen("ranking.dat", "rb");/*se abre el archivo*/
		mvwprintw(config.ranking.win, 0, 1, "[NOM TIEM        DIF]");/*se colocan titulos de cada seccion*/
		if (config.game_board.dif+(config.game_board.altModes*3) > 2)/*se hace el calculo para saber si es un modo con vidas o no*/
		{
			mvwprintw(config.ranking.win, 0, 11, "VIDAS");/*en caso de que sea modo con vidas se muestra esa seccion*/
		}
		if (archivo)/*en caso de que se haya abierto el archivo*/
		{
			for (x = 0; x < config.n_ranks; x++)/*se leen los rankings*/
			{
				fseek(archivo, config.ranks[x].pos*sizeof(Trank), SEEK_SET);/*se toma el indice de rankings correctos*/
				fread(&rank, sizeof(Trank), 1, archivo);/*se lee la estructura de esa posicion del rank*/
				if (rank.type == config.game_board.dif+(config.game_board.altModes*3) && i < 15)/*solo se mostraran los primeros 15 rankings*/
				{
					mvwprintw(config.ranking.win, i+1, 2, "%-4s %-12i %i", rank.name, rank.time, rank.diff+1);/*se imprime el ranking*/
					if (rank.type > 2)/*si es de un modo con vidas se imprimen las vidas*/
					{
						for(h = 0; h < rank.hearts; h++)
						{
							wattron(config.ranking.win, COLOR_PAIR(C_RED));
							mvwprintw(config.ranking.win, i+1, 11+(h*2), "<3");
							wattroff(config.ranking.win, COLOR_PAIR(C_RED));
						}
					}
					i++;/*se incrementa el indice de posicion para dibujar correctamente los rankings*/
				}
			}
			fclose(archivo);/*se cierra el archivo*/
		}
	}
}

/*funcion para dibujar todo en la seccion de estatus*/
void draw_status(Tconfig config)
{
	int i;
	draw_container(config.game_info, C_YELLOW, C_NONE, 1);/*se dibuja el contenedor de estatus del juego*/
	wattron(config.game_info.win, COLOR_PAIR(C_YELLOW));
	mvwprintw(config.game_info.win, 0, (config.game_screen.width/2)-9, "[ESTATUS DEL JUEGO]");/*se dibuja el titulo del contenedor*/
	mvwprintw(config.game_info.win, 2, (config.game_info.width)-10, "[T]: %i", config.game_board.timer);/*se dibuja el tiempo de juego*/
	mvwprintw(config.game_info.win, 2, 3, "[ ]: %i", config.game_board.flags);/*se dibuja el contenedor para mostrar las banderas usables*/
	wattroff(config.game_info.win, COLOR_PAIR(C_YELLOW));
	wattron(config.game_info.win, COLOR_PAIR(C_RED));
	mvwprintw(config.game_info.win, 2, 4, "V");/*se dibuja el icono de banderas*/
	wattroff(config.game_info.win, COLOR_PAIR(C_RED));
	wattron(config.game_info.win, COLOR_PAIR(C_YELLOW));
	mvwprintw(config.game_info.win, 3, 3, "[ ]: %i", config.game_board.freeCells);/*se dibuja el contenedor para mostrar casillas por despejar*/
	wattroff(config.game_info.win, COLOR_PAIR(C_YELLOW));
	wattron(config.game_info.win, A_BOLD | COLOR_PAIR(C_BLACK));
	mvwprintw(config.game_info.win, 3, 4, "#");/*se dibuja el icono de casillas por despejar*/
	wattroff(config.game_info.win, A_BOLD | COLOR_PAIR(C_CYAN));
	/*Smiley*/
	/*son los getos con los que smiley reaccionara a los movimientos o acciones del jugador*/
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
	if (config.game_board.status >= 2)/*en caso de victoria se mostrara esta leyenda en el estatus*/
	{
		wattron(config.game_info.win, COLOR_PAIR(C_YELLOW));
		mvwprintw(config.game_info.win, 4, (config.game_info.width/2)-16, "[Presione [ENTER] para continuar]");
		wattroff(config.game_info.win, COLOR_PAIR(C_YELLOW));
	}
	if (config.game_board.altModes == 1)/*en caso de ser modo con vidas se dibujaran las vidas disponibles*/
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

/*funcion para dibujar el menu de dificultad*/
void draw_diff(Tconfig config)
{
	int i;
	draw_container(config.difficulty, C_BLACK, C_NONE, 1);/*se dibuja el contenedor para el menu de dificultad*/
	for (i = 3; i < 8; i++)
	{
		draw_button(config.difficulty, config.buttons[i], 1, 1);/*se dibujan los botones de dificultad*/
	}
	if (config.game_board.altModes == 1)/*se dibuja el boton alternable de vidas/modo alterno*/
	{
		wattron(config.difficulty.win, COLOR_PAIR(C_RED));
		mvwprintw(config.difficulty.win, (config.difficulty.height/2)+2, (config.difficulty.width/2)+3, "X");/*si esta activo se muestra una X roja*/
		wattroff(config.difficulty.win, COLOR_PAIR(C_RED));
	}
}

/*funcion que controla todas las acciones que realiza el jugador/inputs del juego*/
Tconfig inputs(Tconfig config)
{
	switch (config.game_status)
	{
		case 0:
			config = menu_inputs(config);/*inputs del menu principal*/
		break;
		case 1:
			config = board_inputs(config);/*inputs del tablero*/
		break;
		case 2:
			config = diff_inputs(config);/*inputs del menu de dificultad*/
		break;
	}
	
	return config;/*se retorna la dificultad*/
}

/*funcion para controlar los inputs del menu principal*/
Tconfig menu_inputs(Tconfig config)
{
	int key = getch();/*se espera una entrada por parte del jugador*/
	if (key == 's' || key == KEY_DOWN || key == 'k')/*si se teclea flecha abajo, S o K, se mueve el cursor hacia abajo(cursor que afecta el boton a seleccionar)*/
	{
		config.button_id+=1;
	}
	if (key == 'w' || key == KEY_UP || key == 'l')/*si se teclea flecha arriba, W o L, se mueve el cursor hacia arriba(cursor que afecta que boton se selecciona)*/
	{
		config.button_id-=1;
	}
	if (key == '\n' || key == ' ')/*si se presiona enter o espacio se accede a la accion de cada boton*/
	{
		switch (config.button_id)/*dependiendo del ID actual de boton se inicia la partida, se entra al menu de dificultad o se cierra el juego*/
		{
			/*se inicia el juego en caso de seleccionar el primer boton*/
			case 0:
				config.game_status = 1;/*se cambia el estatus del juego a 1*/
				switch (config.game_board.dif)/*dependiendo de la dificultad escogida o inicial se especifican las opciones del nivel*/
				{
					case 0:
						config = prepare_board(config, 9, 9, 10);/*nivel facil de 9x9 con 10 minas*/
					break;
					case 1:
						config = prepare_board(config, 13, 13, 40);/*nivel normal de 13x13 con 40 minas*/
					break;
					case 2:
						config = prepare_board(config, 17, 17, 100);/*nivel dificil de 17x17 con 100 minas*/
					break;
				}
				change_screen(config, config.game_status);/*se muestra el tablero y se esconden los demas contenedores*/
			break;
			/*se cambia el menu hacia el menu de dificultad*/
			case 1:
				config.game_status = 2;
				config.button_id = 3;/*se selecciona el primer boton del menu de dificultad por orden de creacion*/
				change_screen(config, config.game_status);/* se ocultan todos los contenedores y se muestra solo el menu de dificultad*/
			break;
			case 2:
				key = 'q';/*la tecla Q directamente cierra el juego y se aprovecha para cerrar el juego con el boton seleccionado en el menu*/
			break;
		}
	}
	/*se guarda el boton presionado*/
	config.key = key;
	return config;/*se retorna la configuracion*/
}

/*inputs o entradas del tablero*/
Tconfig board_inputs(Tconfig config){
	int key = getch();/*se espera una entrada por parte del jugador*/
	int x, y;
	if (config.game_board.status < 2)/*se permite el movimiento del cursor del juego si no se ha perdido o ganado partida*/
	{
		if (key == 's' || key == KEY_DOWN || key == 'k')/*movimiento hacia abajo con teclas, S, ABAJO o K*/
		{
			config.game_board.cursor_y+=1;
		}
		if (key == 'w' || key == KEY_UP || key == 'l')/*Movimiento hacia arriba con teclas W, ARRIBA o L*/
		{
			config.game_board.cursor_y-=1;
		}
		if (key == 'd' || key == KEY_RIGHT || key == ';')/*Movimiento hacia la derecha con teclas D, DERECHA o ;*/
		{
			config.game_board.cursor_x+=1;
		}
		if (key == 'a' || key == KEY_LEFT || key == 'j')/*Movimiento hacia la izquierda con teclas A, IZQUIERDA o J*/
		{
			config.game_board.cursor_x-=1;
		}
	}
	/*se ajusta la posicion del cursor en el tablero relativa al contenedor del juego*/
	x = config.game_board.cursor_x+1;
	y = config.game_board.cursor_y+1;
	if (key == 'f' && config.game_board.status == 1)/*si se presiona la tecla F se genera una bandera en caso de que haya una disponible*/
	{
		if (config.game_board.field[y][x][1] == 1)/*si no hay bandera se genera una*/
		{
			if (config.game_board.flags > 0){
				config.game_board.field[y][x][1] = 3;
				config.game_board.flags--;
			}
			config.game_board.smileyStatus = 6;
		}
		else if (config.game_board.field[y][x][1] == 3)/*si hay bandera se retira*/
		{
			config.game_board.field[y][x][1] = 1;
			config.game_board.flags++;
			config.game_board.smileyStatus = 2;
		}
	}
	if (key == '\n' || key == ' ')/*en caso de presionar enter o espacio*/
	{
		if (config.game_board.field[y][x][1] == 1)/*si la casilla esta sin despejar*/
		{
			config.game_board.update_map = 1;/*se permite la actualizacion del mapa*/
		}
		if (config.game_board.status == 2)/*si la partida se pierde presionar enter o espacio salta al menu principal*/
		{
			config.game_status = 0;
			config.button_id = 0;
			change_screen(config, config.game_status);
			key = '0';
		}
		if (config.game_board.status == 3)/*si la partida se gana se permite escoger nombre para usuario en el ranking*/
		{
			show_panel(config.name_input.pane);
			config.game_board.status = 4;
		}
		if (config.game_board.status == 5)/*si el estatus del tablero pasa a la seccion de rankings, estos se muestran*/
		{
			config.game_status = 0;
			change_screen(config, config.game_status);
		}
	}
	if (key == 'q')/*presionar Q en cualquier momento permite salir al menu principal*/
	{
		config.game_status = 0;
		config.button_id = 0;
		change_screen(config, config.game_status);
		key = '0';
	}
	if (config.game_board.status == 4)/*si el estatus del tablero pasa a la seccion de introducir nombre se permite*/
	{
		config = input_name(config, key);
	}
	config.key = key;/*se guarda la tecla*/
	return config;/*se retorna la configuracion*/
}

/*funcion con el algoritmo para despejar niebla por perimetros*/
Tconfig spiral_clear(Tconfig config)
{
	/*se designan variables de coordenadas y variables de ayuda para el algoritmo*/
	int i, x, y, dir = 0, current_step = 1, max_steps = 1, turn_cont = 0;
	short place = 0;/*variable que guarda si el espacio es valido o no para despejar*/
	x = config.game_board.width/2;/*se ubica la posicion en el centro del mapa/tablero*/
	y = config.game_board.height/2;
	x++;/*se incrementa en 1 para que sea proporcional*/
	y++;
	for (i = 0; i < (config.game_board.width*config.game_board.height); i++)/*se hace un loop que ejecutara un bloque de código una vez por cada casilla en el tablero*/
	{
		/*se comprueban las casillas adyacentes para ver si estan libres*/
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

		/*si la casilla tiene un numero es invalida para el algoritmo*/
		if (config.game_board.field[y][x][0] > 0){
			place = 0;
		}
		/*si la casilla esta despejada con una condicional de nivel dos o un numero, tambien es invalida*/
		if (config.game_board.field[y][x][1] == 2){
			place = 0;
		}
		/*si la casilla tiene una mina, es invalida tambien*/
		if (config.game_board.field[y][x][0] == -1){
			place = 0;
		}
		/*si la casilla tiene bandera es invalida*/
		if (config.game_board.field[y][x][1] == 3){
			place = 0;
		}
		/*en caso de que la casilla sea valida se despeja*/
		if (place == 1)
		{
			if (config.game_board.field[y][x][1] == 1)
			{
				config.game_board.field[y][x][1] = 0;
			}
		}
		/*se reinicia la variable de validacion en 0*/
		place = 0;
		/*dependiendo de la direccion actual se movera la posicion del cursor para validar*/
		switch (dir)
		{
			case 0:/*si la direccion es 0 se movera a la derecha*/
				x++;
			break;
			case 1:/*si la direccion es 1 se movera hacia abajo*/
				y++;
			break;
			case 2:/*si la posicion es 2 se movera a la izquierda*/
				x--;
			break;
			case 3:/*si la posicion es 3 se movera hacia arriba*/
				y--;
			break;
		}
		/*si ya se han superad los pasos maximos se reinician y se incrementa el contador para escoger el cambio de direccion*/
		if (current_step >= max_steps)
		{
			current_step = 0;
			turn_cont++;
			dir++;
		}
		/*si ya se ha alcanzado el maximo de cambios de direccion que es 2, se incrementa el conteo maximo de pasos y se reinicia el contador*/
		if (turn_cont >= 2)
		{
			turn_cont = 0;
			max_steps++;
		}
		/*si la direccion es mayor que 3 se reinicia*/
		if (dir > 3)
		{
			dir = 0;
		}
		/*se incrementa el contador de pasos o movimientos*/
		current_step++;
	}
	/*se retorna la configuracion*/
	return config;
}

/*funcion para despejar la niebla o casillas libres a mostrar*/
Tconfig clear_mist(Tconfig config)
{
	int i, x, y, place = 0;
	for (i = 0; i <= 10; i++)
	{
		config = spiral_clear(config);/*se llama el despeje de espiral 10 veces para asegurar*/
	}
	
	/*seccion que comprueba casillas con numero a despejar*/
	for (y = 1; y <= config.game_board.height; y++)
	{
		for (x = 1; x <= config.game_board.width; x++) 
		{
			if (config.game_board.field[y][x][0] > 0)/*si la casilla tiene un numero*/
			{
				if (config.game_board.field[y][x][1] != 3)/*si la casilla no esta marcada con una bandera*/
				{
					if (config.game_board.field[y][x][1] != 2)/*si la casilla no esta marcada como casilla despejada de nivel 2*/
					{
						/* se comprueba que las casillas adyacentes esten despejadas, por lo menos una*/
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
			/*si la casilla con numero tiene una casilla adyacente despejada por lo menos, es valida para despejar en nivel 2*/
			if (place == 1)
			{
				if (config.game_board.field[y][x][1] != 2)/* si no esta despejada en nivel 2 entonces se despeja*/
				{
					config.game_board.field[y][x][1] = 2;
				}
			}
		place = 0;/*se reinicia la variable de validacion*/
		}
	}
	return config;/* se retorna la configuracion*/
}

/*funcion para mostrar las bombas*/
Tconfig show_bombs(Tconfig config)
{
	int x, y;
	/*se comprueba cada una de las casillas*/
	for (y = 1; y <= config.game_board.height; y++)
	{
		for (x = 1; x <= config.game_board.width; x++)
		{
			/*si la casilla tiene bomba en cuestion*/
			if (config.game_board.field[y][x][0] == -1)
			{
				if (config.game_board.field[y][x][1] == 1)/*si la casilla no esta despejada se despeja*/
				{
					config.game_board.field[y][x][1] = 0;
				}
				if (config.game_board.field[y][x][1] == 3)/* si la casilla tiene bandera la bomba se marca como encontrada*/
				{
					config.game_board.field[y][x][1] = 4;
				}
			}
		}
	}
	/*se retorna la configuracion*/
	return config;
}

/*configuracion para esperar los inputs o entradas de teclado del menu de dificultad*/
Tconfig diff_inputs(Tconfig config)
{
	int key = getch();/* se espera una tecla o entrada por parte del usuario*/
	if (key == 's' || key == KEY_DOWN || key == 'k')/*teclas S, abajo y K para moverse hacia abajo en el menu*/
	{
		config.button_id+=1;
	}
	if (key == 'w' || key == KEY_UP || key == 'l')/* teclas W, arriba y L para moverse hacia arriba*/
	{
		config.button_id-=1;
	}
	if (key == '\n' || key == ' ')/*teclas enter y espacio para escoger la dificultad*/
	{
		switch (config.button_id)
		{
			case 3:
				config.game_board.dif = 0;/*boton 3 con dificultad facil*/
			break;
			case 4:
				config.game_board.dif = 1;/*boton 4 con dificultad normal*/
			break;
			case 5:
				config.game_board.dif = 2;/*boton 5 con dificultad dificultad*/
			break;
			case 6:
				config.game_board.altModes = !config.game_board.altModes;/*boton alternable para el modo con vidas*/
			break;
		}
		if (config.button_id != 6)/* si se escoge el modo de vidas no sale automaticamente fuera*/
		{
			config.game_status = 0;
			change_screen(config, 0);
			config.button_id = 0;
		}
	}
	config.key = key;/*se guarda la tecla*/
	return config;/*se retorna la configuracion*/
}

/*funcion para preparar el tablero*/
Tconfig prepare_board(Tconfig config, int width, int height, int mines)
{
	int y, x;
	if (config.game_board.altModes == 1)/*si se ha escogido el modo con vidas previamente se establece el contador en 3 vidas*/
	{
		config.game_board.hearts = 3;
	}
	else/*de lo contrario es solo una vida en modo normal, muerte subita en la primer mina*/
	{
		config.game_board.hearts = 1;
	}
	config.game_board.timer = 0;/*se inicia el timer en cero*/
	config.game_board.delta = 0;/*se inicia el delta en cero*/
	config.game_board.smileyStatus = 2;/*se establece el estado inicial de smiley*/
	config.game_board.status = 0;/*se establece el estatus del tablero*/
	config.game_board.update_map = 0;/*se indica que el tablero aun no actualizara la niebla*/
	config.game_board.width = width;/*se obtiene el ancho del tablero*/
	config.game_board.height = height;/*se obtiene la altura del tablero*/
	
	config.game_board.cursor_x = width/2;/*se coloca el cursor en el centro*/
	config.game_board.cursor_y = height/2;
	
	config.game_screen.height = (height*2)+1;/*se decide la posicion del contenedor (altura)*/
	config.game_screen.width = (config.game_screen.height*2)-1;/*se decide la anchura del contenedor*/
	/*se designan parametros temporales*/
	for (y = 0; y < 19; y++)
	{
		for (x = 0; x < 19; x++)
		{
			config.game_board.field[y][x][0] = -2;
			config.game_board.field[y][x][1] = 1;
		}
	}
	
	/*se designa la anchura del contenedor con info*/
	config.game_info.width = (config.game_screen.height*2)-1;
	config.game_info.y = -(height+3);/*se designa la posicion de altura del contenedor de info*/
	config.game_board.mines = mines;/*se designa la cantidad de minas en el mapa*/
	config.game_board.flags = mines;/*se designa la cantidad de banderas disponibles*/
	config.game_board.freeCells = (width*height)-mines;/*se designa la cantidad de celdas a despejar*/
	
	return config;/*se retorna la configuracion*/
}

/*funcion para generar el mapa en el primer movimiento*/
Tconfig gen_board(Tconfig config)
{
	int y, x, mines, cont;
	mines = config.game_board.mines;/*se obtiene la cantidad de minas en base a la dificultad*/
	config.game_board.delta = time(NULL);/*se inicia el delta para saber cuanto tiempo llevara el reloj o cronometro*/
	while(mines > 0)/*mientras el contador de minas aún no sea cero se seguiran generando*/
	{
		if ((rand()%10) == 0)/*si un numero aleatorio del 0 al 9 cae en 0 se escoge un lugar al azar donde poner una mina*/
		{
			y = (rand()%config.game_board.height);/*se escoge la posicion al azar en el tablero*/
			x = (rand()%config.game_board.width);
			if (config.game_board.field[y+1][x+1][0] != -1)/*si no hay una mina en la posicion*/
			{
				if (x != config.game_board.cursor_x && y != config.game_board.cursor_y)/*si la mina no coincide cerca del cursor*/
				{
					if (x != config.game_board.cursor_x+1 && y != config.game_board.cursor_y)
					{
						if (x != config.game_board.cursor_x-1 && y != config.game_board.cursor_y)
						{
							config.game_board.field[y+1][x+1][0] = -1;/*se genera mina*/
							mines--;/*se reduce el contador de minas*/
						}
					}
				}
			}
		}
	}
	
	/*se designa el numero de cada casilla en base a las minas a la redonda*/
	for (y = 1; y <= config.game_board.height; y++)
	{
		for (x = 1; x <= config.game_board.width; x++)
		{
			cont = 0;/*el contador inicial es de 0*/
			/*se comprueban las casillas adyacentes*/
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
			/*si la casilla aun no se ha designado se le asigna el numero en base a sus minas adyacentes*/
			if (config.game_board.field[y][x][0] == -2)
			{
				config.game_board.field[y][x][0] = cont;
			}
		}
	}
	return config;
}

/*funcion para introducir el nombre de jugador al finalizar una partida en victoria*/
Tconfig input_name(Tconfig config, int key)
{
	FILE *archivo;/*variable para ubicar el archivo*/
	Trank ranking;
	if (key > 64 && key < 91 || key > 96 && key < 123)/*si la tecla presionada es una letra*/
	{
		if (config.name_cid < 3)/*mientras la posicion sea menor al numero de caracteres permitidos*/
		{
			if (key > 96)
			{
				key -= 32;/* si es letra minuscula se hace mayuscula automaticamente*/
			}
			config.name[config.name_cid] = key;/*se coloca la letra tecleada en la posicion del nombre sin llenar*/
			config.name_cid++;/*se incrementa la variable para la posicion a llenar*/
		}
	}
	if (key == KEY_BACKSPACE && config.name_cid > 0)/*seccion que permite borrar una letra en caso de escribir mal*/
	{
		config.name_cid--;/*se decrese el contador*/
		config.name[config.name_cid] = '_';/*se reemplaza la letra de nuevo por el simbolo vacio*/
	}
	if ((key == '\n' || key == ' ') && config.name_cid == 3)/*si se han llenado los espacions presionar enter para admitir*/
	{
		if (config.game_board.status == 4)/*se cambia el estatus para mostrar el ranking*/
		{
			hide_panel(config.name_input.pane);/*se esconde el contenedor para colocar nombre*/
			show_panel(config.ranking.pane);/*se muestra el panel de los rankings*/
			config.game_board.status = 5;/*se cambia el estatus a 5 para permitir la carga de rankings*/
			archivo = fopen("ranking.dat", "a+b");/*se abre el archivo de los rankings*/
			if (archivo)/*si el archivo se abrio con exito se comienza a configurar el rank nuevo*/
			{
				ranking.type = config.game_board.dif+(config.game_board.altModes*3);/*se calcula en que modo y dificultad pertenecera*/
				ranking.name[0] = config.name[0];/*se guarda el nombre*/
				ranking.name[1] = config.name[1];
				ranking.name[2] = config.name[2];
				ranking.name[3] = config.name[3];
				ranking.diff = config.game_board.dif;/*se guarda la dificultad*/
				ranking.time = config.game_board.timer;/*se guarda el tiempo del cronometro*/
				ranking.hearts = config.game_board.hearts;/*se guardan la cantidad de vidas*/
				fwrite(&ranking, sizeof(ranking), 1, archivo);/*se escribe en el archivo*/
				fclose(archivo);/*se cierra el archivo*/
			}
			config = load_rank(config);/*se llama la funcion para cargar los rankings*/
			key = '0';
		}
	}
	return config;/*se retorna la configuracion*/
}

/*funcion para cargar rankings*/
Tconfig load_rank(Tconfig config)
{
	FILE *archivo;
	Trank rank;
	int i = 0;
	archivo = fopen("ranking.dat", "rb");/*se abre el archivo*/
	if (archivo)
	{
		while (fread(&rank, sizeof(Trank), 1, archivo))/*se leen los rankings*/
		{
			config.ranks[i].time = rank.time;/*se guardan en el vector de indices*/
			config.ranks[i].pos = i;
			i++;
		}
		config.n_ranks = i;/*se guarda la cantidad de rankings final*/
		fclose(archivo);/*se cierra el archivo*/
		shellSort(config.ranks, i);/*se ordenan los rankings en base al tiempo*/
	}
	return config;/*se retorna la configuracion*/
}

/*funcion para ordenar (shell sort)*/
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
