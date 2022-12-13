#include <ncurses.h>
#include <panel.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*Configura colores esenciales*/
#define C_NONE		-1
#define C_BLACK		1
#define C_RED		2
#define C_GREEN		3
#define C_YELLOW	4
#define C_BLUE		5
#define C_MAGENTA	6
#define C_CYAN		7
#define C_WHITE		8

/*La estructura para manejar ventanas*/
typedef struct _tcontainer
{
	int term_w, term_h;/*variables para guardar el tamano de la terminal*/
	int width, height;/*variables para guardar el tamaño del contenedor*/
	int x, y;/*variables para guardar la posición del contenedor*/
	int fg, bg;/*variables para guardar el color y fondo del contenedor*/
	int ac;/*variable para guardar modo alterno de color*/
	int type;/*variable para guardar tipo de contenedor*/
	WINDOW* win;/*variable de ncurses para guardar ventana del contenedor*/
	PANEL* pane;/*variable de ncurses para guardar panel del contenedor*/
}Tcontainer;

/*La estructura para controlar botones*/
typedef struct _tbutton
{
	int x, y;/*varialbe para guardar la posición del botón*/
	char text[50], alt_text[50];/*variables para guardar el texto normal y alternativo del botón*/
	int fg, bg, ac;/*variables para guardar los colores del botón*/
}Tbutton;

/*La estructura para controlar el tablero de juego*/
typedef struct _board
{
	int dif;/*variable para guardar la dificultad del tablero/nivel*/
	int width, height;/*variable para guardar el tamaño del tablero/nivel*/
	int cursor_x, cursor_y;/*variable para guardar el cursor para jugar*/
	int field[19][19][2];/*matriz tridimensional para guardar las casillas y sus propiedades*/
	int status;/*variable para guardar el estatus de la partida*/
	int update_map;/*variable para saber cuando actualizar el mapa en base a las acciones del jugador*/
	int mines;/*variable para saber la cantidad de minas en el tablero*/
	int flags;/*variable para saber la cantidad de banderas en el tablero*/
	int freeCells;/*variable para saber la cantidad de casillas libres en el tablero*/
	int hearts;/*variable para saber la cantidad de vidas en partida*/
	int smileyStatus;/*variable para saber el estado de smiley*/
	int altModes;/*variable para saber si se escoge el modo con vidas*/
	int delta;/*variable para saber la diferencia de tiempo al inicio de partida*/
	int timer;/*variable para guardar el tiempo como cronometro*/
}Tboard;

/*estructura para guardar ranking*/
typedef struct _trank
{
	int type;/*saber que tipo de ranking es en base al modo y dificultad*/
	char name[4];/*nombre del jugador*/
	int diff;/*dificultad de la partida*/
	int time;/*tiempo de juego*/
	int hearts;/*cantidad de corazones o vidas*/
}Trank;

/*estructura para crear el indice de rankings*/
typedef struct _tind
{
	int time;/*se toma en consideracion el tiempo como variable llave*/
	int pos;/*se guarda la posición del ranking*/
}Tind;

/*La estructura para controlar configuraciones generales del juego*/
typedef struct _tconfig
{
	int game_status;/*estado general del juego*/
	int button_id;/*id para saber que boton se tiene seleccionado*/
	int key;/*variable para saber que tecla se presiono*/
	Tcontainer menu, game_screen, difficulty;/*contenedores de menu, tablero y menu de dificultad*/
	Tcontainer game_info, name_input, ranking;/*contenedores de info o estatus del juego, input para poner el nombre de usuario y ranking*/
	Tbutton buttons[8];/*botones del juego*/
	Tboard game_board;/*tablero del juego*/
	char name[4];/*nombre de jugador*/
	int name_cid;/*posición de cursor sobre el nombre del jugador*/
	Tind ranks[3000];/*cantidad de rankings a cargar*/
	int n_ranks;/*cantidad de rankings cargados*/
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
