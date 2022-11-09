#include <ncurses.h>
#include <panel.h>

#define C_NONE		-1
#define C_BLACK		0
#define C_RED		1
#define C_GREEN		2
#define C_YELLOW	3
#define C_BLUE		4
#define C_MAGENTA	5
#define C_CYAN		6
#define C_WHITE		7

typedef struct _twindow{
	int term_w, term_h;
	int width;
	int height;
	int x, y;
	int fg, bg;
	int ac;
	int type;
	WINDOW* win;
	PANEL* pane;
	
}TWindow;

void lncurses();
void game_loop(int update(), void draw());

void definePane(int x, int y, int width, int height, int type);

TWindow create_window(int x, int y, int width, int height, int fg, int bg, int ac, int type, int term_w, int term_h);

void colorizeWin(TWindow window, int fg, int bg, int ac);
