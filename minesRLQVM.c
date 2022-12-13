#include "minesRLQVM.h"

/*funcion para cargar propiedades de ncurses*/
void lncurses()
{
	initscr();/*se inicia el modo de ncurses*/
	#ifdef _WIN32/*para windows*/
	stdscr = initscr();
	DWORD dwMode = 0;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut != INVALID_HANDLE_VALUE)
	{
		if (GetConsoleMode(hOut, &dwMode))
		{
			SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		}
	}
	#endif
	noecho();/*no se dibuja la entrada del teclado*/
	/*cbreak();*//*desactivar el buffering*/
	curs_set(FALSE);/*se oculta el cursor de la terminal*/
	keypad(stdscr,TRUE);/*habilitar mayor cantidad de teclas*/
	use_default_colors();/*permitir el uso de colores*/
	start_color();/*iniciar seleccion de colores*/
	/*definir pares de colores*/
	init_pair(1, 0, -1);/*negro*/
	init_pair(2, 1, -1);/*rojo*/
	init_pair(3, 2, -1);/*verde*/
	init_pair(4, 3, -1);/*amarillo*/
	init_pair(5, 4, -1);/*azul*/
	init_pair(6, 5, -1);/*magenta*/
	init_pair(7, 6, -1);/*cyan*/
	init_pair(8, 7, -1);/*blanco*/
}

/*funcion mas importante de todas, el ciclo del juego*/
void game_loop(Tconfig update(), void draw(), Tconfig inputs(), Tconfig config)
{
	do
	{
		config = update(config);/*se actualiza toda la logica del juego*/
		draw(config);/*se dibuja todo despues de actualizar la logica*/
		
		update_panels();/*se actualizan todos los paneles*/
		doupdate();/*se actualizan las funcionalidades de ncurses*/
		
		config = inputs(config);/*se pide el input o accion por parte del jugador*/
	}
	while(config.key != 'q');/*si la tecla presionada es Q termina el juego*/
}

/*funcion para crear contenedores*/
Tcontainer create_container(int x, int y, int width, int height, int fg, int bg, int ac, int type, int term_w, int term_h)
{
	Tcontainer container;/*se crea estructura temporal*/
	container.x = x;/*se designa la ubicacion*/
	container.y = y;
	container.width = width;/*se designa el tamaño*/
	container.height = height;
	container.fg = fg;/*se designan los colores*/
	container.bg = bg;
	container.ac = ac;
	container.type = type;/*se designa el tipo*/
	container.term_w = term_w;/*se guarda el tamaño del a terminal*/
	container.term_h = term_h;
	
	if (type)/*si es de tipo uno se dibuja en el centro*/
	{
		container.win = newwin(height, width, y+(term_h/2)-(height/2), x+(term_w/2)-(width/2));
	}
	else/*si es de tipo 0 o otro diferente de q se dibuja desde la esquina superior izquierda*/
	{
		container.win = newwin(height, width, 0, 0);
	}
	container.pane = new_panel(container.win);/*se completan las funciones de ncurses*/
	draw_container(container, fg, bg, ac);
	return container;/*se retorna el contenedor*/
}

/*funcion para dibujar contenedores*/
void draw_container(Tcontainer container, int fg, int bg, int ac)
{
	container.ac = ac;/*se cambian los colores del contenedor*/
	container.fg = fg;
	container.bg = bg;
	werase(container.win);/*se borra la ventana*/
	wresize(container.win, container.height, container.width);/*se actualiza el tamaño*/
	if (container.type)/*si el tipo de contenedor es 1 se dibuja desde el centro*/
	{
		mvwin(container.win, container.y+(container.term_h/2)-(container.height/2), container.x+(container.term_w/2)-(container.width/2));
	}
	if (container.ac)/*si el aspecto del contenedor es alterno se dibuja con colores gruesos*/
	{
		wattron(container.win, A_BOLD | COLOR_PAIR(container.fg));
		box(container.win, 0, 0);
		wattroff(container.win, A_BOLD | COLOR_PAIR(container.fg));
	}
	else/*si el aspecto es normal se usan colores base*/
	{
		wattron(container.win, COLOR_PAIR(container.fg));
		box(container.win, 0, 0);
		wattroff(container.win, COLOR_PAIR(container.fg));
	}
}

/*funcion para crear botones*/
Tbutton create_button(int x, int y, char text[], int fg, int bg, int ac)
{
	Tbutton button;/*boton temporal*/
	char altText[50];/*variable para texto alterno*/
	int len = 0;/*variable para saber la longitud del texto*/
	len = strlen(text);/*se calcula la longitud del texto*/
	altText[0] = '[';/*se prepara la version alternativa rodeadad con corchetes []*/
	altText[1] = '\0';
	strcat(altText, text);
	altText[len+1] = ']';
	altText[len+2] = '\0';
	button.x = x;/*se calcula la posicion del boton*/
	button.y = y;
	strcpy(button.text, text);/*se guardan los textos*/
	strcpy(button.alt_text, altText);
	button.fg = fg;/*se guardan los colores*/
	button.bg = bg;
	button.ac = ac;
	
	return button;/*se retorna el boton*/
}

/*funcion para dibujar un boto*/
void draw_button(Tcontainer container, Tbutton button, int type_x, int type_y)
{
	int len = 0, xx = 0, yy = 0;/*variables para la posicion y longitud del texto*/
	if (button.ac)/*si el boton esta seleccionado se muestra la version alternativa*/
	{
		len = strlen(button.alt_text);/*longitud actualizada*/
	}
	else/*si no esta seleccionado se muestra normal*/
	{
		len = strlen(button.text);/*longitud actualizada*/
	}
	
	/*altura del boton relativa en caso de quererse en el centro del eje vertical*/
	if (type_y)
	{
		yy = (container.height/2);
	}
	/*posicion del boton relativa en caso de quererse en el centro del eje horizontal*/
	if (type_x)
	{
		xx = (container.width/2)-(len/2);
	}
	
	/*se dibuja con un color distinto en caso de estar seleccionado*/
	wattron(container.win, COLOR_PAIR(button.fg));
	if (button.ac)
	{
		mvwprintw(container.win, button.y+yy, button.x+xx, "%s", button.alt_text);
	}
	else/*se dibuja con su color normal en caso de no estar seleccionado*/
	{
		mvwprintw(container.win, button.y+yy, button.x+xx, "%s", button.text);
	}
	wattroff(container.win, COLOR_PAIR(button.fg));
}
