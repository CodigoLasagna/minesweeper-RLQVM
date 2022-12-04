#include "minesRLQVM.h"

void lncurses(){
	initscr();
	#ifdef _WIN32
	stdscr = initscr();
	DWORD dwMode = 0;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut != INVALID_HANDLE_VALUE) {
		if (GetConsoleMode(hOut, &dwMode)) {
			SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		}
	}
	#endif
	noecho();
	cbreak();
	curs_set(FALSE);
	/*nodelay(stdscr, TRUE);*/
	keypad(stdscr,TRUE);
	use_default_colors();
	start_color();
	init_pair(1, 0, -1);
	init_pair(2, 1, -1);
	init_pair(3, 2, -1);
	init_pair(4, 3, -1);
	init_pair(5, 4, -1);
	init_pair(6, 5, -1);
	init_pair(7, 6, -1);
	init_pair(8, 7, -1);
}

void game_loop(Tconfig update(), void draw(), Tconfig inputs(), Tconfig config)
{
	do{
		config = update(config);
		draw(config);
		
		update_panels();
		doupdate();
		
		config = inputs(config);
	}while(config.key != 'q');
}

Tcontainer create_container(int x, int y, int width, int height, int fg, int bg, int ac, int type, int term_w, int term_h)
{
	Tcontainer container;
	container.x = x;
	container.y = y;
	container.width = width;
	container.height = height;
	container.fg = fg;
	container.bg = bg;
	container.ac = ac;
	container.type = type;
	container.term_w = term_w;
	container.term_h = term_h;

	if (type){
		container.win = newwin(height, width, y+(term_h/2)-(height/2), x+(term_w/2)-(width/2));
	}else{
		container.win = newwin(height, width, 0, 0);
	}
	container.pane = new_panel(container.win);
	draw_container(container, fg, bg, ac);
	return container;
}
void draw_container(Tcontainer container, int fg, int bg, int ac){
	container.ac = ac;
	container.fg = fg;
	container.bg = bg;
	werase(container.win);
	wresize(container.win, container.height, container.width);
	if (container.type == true){
		mvwin(container.win, container.y+(container.term_h/2)-(container.height/2), container.x+(container.term_w/2)-(container.width/2));
	}
	if (container.ac == false){
		wattron(container.win, COLOR_PAIR(container.fg));
		box(container.win, 0, 0);
		wattroff(container.win, COLOR_PAIR(container.fg));
	}else{
		wattron(container.win, A_BOLD | COLOR_PAIR(container.fg));
		box(container.win, 0, 0);
		wattroff(container.win, A_BOLD | COLOR_PAIR(container.fg));
	}
}

Tbutton create_button(int x, int y, char text[], int fg, int bg, int ac){
	Tbutton button;
	char altText[50];
	int len = 0;
	len = strlen(text);
	altText[0] = '[';
	altText[1] = '\0';
	strcat(altText, text);
	altText[len+1] = ']';
	altText[len+2] = '\0';
	button.x = x;
	button.y = y;
	strcpy(button.text, text);
	strcpy(button.alt_text, altText);
	button.fg = fg;
	button.bg = bg;
	button.ac = ac;
	
	return button;
}

void draw_button(Tcontainer container, Tbutton button, int type_x, int type_y){
	int len = 0, xx = 0, yy = 0;
	if (button.ac){
		len = strlen(button.alt_text);
	}else{
		len = strlen(button.text);
	}
	
	if (type_y){
		yy = (container.height/2);
	}
	if (type_x){
		xx = (container.width/2)-(len/2);
	}
	
	wattron(container.win, COLOR_PAIR(button.fg));
	if (button.ac){
		mvwprintw(container.win, button.y+yy, button.x+xx, "%s", button.alt_text);
	}else{
		mvwprintw(container.win, button.y+yy, button.x+xx, "%s", button.text);
	}
	wattroff(container.win, COLOR_PAIR(button.fg));
}
