#include "minesRLQVM.h"


void lncurses(){
	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);
	nodelay(stdscr, TRUE);
	keypad(stdscr,TRUE);
	use_default_colors();
	start_color();
}

void game_loop(int update(), void draw())
{
	while (update) {
		draw();
		update_panels();
		doupdate();
	}
}

TWindow create_window(int x, int y, int width, int height, int fg, int bg, int ac, int type, int term_w, int term_h)
{
	TWindow window;
	window.x = x;
	window.y = y;
	window.width = width;
	window.height = height;
	window.fg = fg;
	window.ac = ac;
	window.type = type;
	if (type){
		window.win = newwin(height, width, y+(term_h/2)-(height/2), x+(term_w/2)-(width/2));
	}else{
		window.win = newwin(height, width, 0, 0);
	}
	window.pane = new_panel(window.win);
	colorizeWin(window, fg, bg, ac);
	return window;
}
void colorizeWin(TWindow window, int fg, int bg, int ac){
	window.ac = ac;
	window.fg = fg;
	window.bg = bg;
	werase(window.win);
	if (window.type == false)
		mvwin(window.win, window.y+(window.term_w/2)-(window.width/2), window.x+(window.term_w/2)-(window.width/2));
	wresize(window.win, window.height, window.width);
	init_pair(window.fg, window.fg, window.bg);
	if (window.ac == false){
		wattron(window.win, COLOR_PAIR(window.fg));
		box(window.win, 0, 0);
		wattroff(window.win, COLOR_PAIR(window.fg));
	}else{
		wattron(window.win, A_BOLD | COLOR_PAIR(window.fg));
		box(window.win, 0, 0);
		wattroff(window.win, A_BOLD | COLOR_PAIR(window.fg));
	}
}
