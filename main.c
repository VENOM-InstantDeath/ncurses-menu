#include <ncurses.h>
#include "menu.h"

int print(WINDOW* win, struct Data* data, void* x) {
	char* pr = (char*)x;
	mvwaddstr(win, 6,0, pr);
	wrefresh(win);
	napms(1000);
	return 1;
}
int mex(WINDOW* win, struct Data* data, void* x) {return 0;}

int main() {
	WINDOW* stdscr = initscr();
	start_color();
	use_default_colors();
	curs_set(0);
	noecho();
	set_escdelay(100);
	keypad(stdscr, 1);
	scrollok(stdscr, 1);
	init_pair(1, 0, 7);
	struct Data data;
	int (*foo[])(WINDOW*,struct Data*,void*) = {print,print,print,print,print,print,mex};
	void *args[] = {"Watefoc", "nobro", "xd", "xd", "xd", "xd",NULL};
	char* ls[] = {"Print watefoc", "Print nobro", "xd", "xd", "xd", "xd", "Exit"};
	struct Callback cb = {foo, args, ls, 7};
	for (;;) {
		if (menu(stdscr, &data, cb, display)) {
			wmove(stdscr, 0, 0); wclrtobot(stdscr);
			continue;
		} else break;
	}
	endwin();
}
