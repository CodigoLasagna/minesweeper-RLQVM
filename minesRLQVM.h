#include <ncurses.h>
#include <panel.h>
#include <string.h>

#define C_NONE		-1
#define C_BLACK		0
#define C_RED		1
#define C_GREEN		2
#define C_YELLOW	3
#define C_BLUE		4
#define C_MAGENTA	5
#define C_CYAN		6
#define C_WHITE		7

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

typedef struct _tbutton{
	int x, y;
	char text[50], alt_text[50];
	int fg, bg, ac;
}Tbutton;

typedef struct _tconfig{
	int game_status;
	int button_id;
}Tconfig;

void lncurses();
void game_loop(int update(), void draw(), int inputs(), Tconfig config);

Tcontainer create_container(int x, int y, int width, int height, int fg, int bg, int ac, int type, int term_w, int term_h);

void draw_container(Tcontainer container, int fg, int bg, int ac);

Tbutton create_button(int x, int y, char text[], int fg, int bg, int ac);
void draw_button(Tcontainer container, Tbutton button, int type_x, int type_y);
