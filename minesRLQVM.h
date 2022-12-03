#include <ncurses.h>
#include <panel.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*Configura colores esenciales*/
#define C_NONE		-1
#define C_BLACK		0
#define C_RED		1
#define C_GREEN		2
#define C_YELLOW	3
#define C_BLUE		4
#define C_MAGENTA	5
#define C_CYAN		6
#define C_WHITE		7

/*La estructura para manejar ventanas*/
typedef struct _tcontainer{
	int term_w, term_h;
	int width;
	int height;
	int x, y;
	int fg, bg;
	int ac;
	int type;
	WINDOW* win;
	PANEL* pane;
	
}Tcontainer;

/*La estructura para controlar botones*/
typedef struct _tbutton{
	int x, y;
	char text[50], alt_text[50];
	int fg, bg, ac;
}Tbutton;

/*La estructura para controlar el tablero de juego*/
typedef struct _board{
	int dif;
	int width, height;
	int cursor_x, cursor_y;
	int field[19][19][2];
	int status;
	int update_map;
	int mines;
	int flags;
	int freeCells;
	int hearts;
	int smileyStatus;
}Tboard;

/*La estructura para controlar configuraciones generales del juego*/
typedef struct _tconfig{
	int game_status;
	int button_id;
	int key;
	Tcontainer menu, game_screen, difficulty;
	Tcontainer game_info;
	Tbutton buttons[8];
	Tboard game_board;
}Tconfig;

/*cargar funciones esenciales de Ncurses*/
void lncurses();
/*La funcion más importante, controla el flujo del juego*/
void game_loop(Tconfig update(), void draw(), Tconfig inputs(), Tconfig config);

/*permite crear ventanas*/
Tcontainer create_container(int x, int y, int width, int height, int fg, int bg, int ac, int type, int term_w, int term_h);

/*permite dibujar ventanas*/
void draw_container(Tcontainer container, int fg, int bg, int ac);

/*permite crear botones*/
Tbutton create_button(int x, int y, char text[], int fg, int bg, int ac);
/*Permite dibujar botones*/
void draw_button(Tcontainer container, Tbutton button, int type_x, int type_y);
