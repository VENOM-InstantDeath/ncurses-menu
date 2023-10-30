#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

struct Data {
};
struct Callback {
	int (**foo)(WINDOW*, struct Data*, void*);
	void** args;
	char** ls;
	int nmemb;
};

int print(WINDOW* win, struct Data* data, void* x) {
	char* pr = (char*)x;
	mvwaddstr(win, 6,0, pr);
	wrefresh(win);
	napms(1000);
	return 1;
}
int mex(WINDOW* win, struct Data* data, void* x) {return 0;}

void display(WINDOW* win, struct Callback cb, int p, int start, int stop) {
	if (stop==-1) {
		mvwaddstr(win, p, 0, cb.ls[start]);
		return;
	}
	if (stop>cb.nmemb) stop=cb.nmemb;
	for (int i=start; i<stop; i++) {
		mvwaddstr(win, i-start, 0, cb.ls[i]);
	}
}

int menu(WINDOW* win, struct Data* data, struct Callback cb, void (*dcb)(WINDOW*,struct Callback,int,int,int)) {
	int p = 0;
	int y,x; getmaxyx(win,y,x);
	int top=y;
	dcb(win, cb, p, top-y, y);
	wattron(win, COLOR_PAIR(1));
	if (cb.nmemb) dcb(win, cb, p, 0, -1);
	wattroff(win, COLOR_PAIR(1));
	for (;;) {
		int ch = wgetch(win);
		switch (ch) {
			case 27:
				return 0;
			case 10:
				if (!cb.nmemb) break;
				return cb.foo[p](win, data, cb.args[p]);
			case KEY_UP:
				if (!cb.nmemb||!p) break;
				dcb(win, cb, p, p, -1);
				wattron(win, COLOR_PAIR(1));
				dcb(win, cb, p-1, p-1, -1);
				wattroff(win, COLOR_PAIR(1));
				p--;
				break;
			case KEY_DOWN:
				if (!cb.nmemb||p==cb.nmemb-1) break;
				if (p==top-1) {
					// scroll
				}
				dcb(win, cb, p, p, -1);
				wattron(win, COLOR_PAIR(1));
				dcb(win, cb, p+1, p+1, -1);
				wattroff(win, COLOR_PAIR(1));
				p++;
				break;
		}
	}
	return 1;
}

int main() {
	WINDOW* stdscr = initscr();
	start_color();
	use_default_colors();
	curs_set(0);
	noecho();
	set_escdelay(100);
	keypad(stdscr, 1);
	init_pair(1, 0, 7);
	struct Data data;
	int (*foo[])(WINDOW*,struct Data*,void*) = {print,print,mex};
	void *args[] = {"Watefoc", "nobro", NULL};
	char* ls[] = {"Print watefoc", "Print nobro", "Exit"};
	struct Callback cb = {foo, args, ls, 3};
	for (;;) {
		if (menu(stdscr, &data, cb, display)) {
			wmove(stdscr, 0, 0); wclrtobot(stdscr);
			continue;
		} else break;
	}
	endwin();
}
