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
	int e=0;
	if (stop>cb.nmemb) stop=cb.nmemb;
	for (int i=start; i<stop; i++) {
		mvwaddstr(win, e, 0, cb.ls[i]);
		e++;
	}
}

int menu(WINDOW* win, struct Data* data, struct Callback cb, void (*dcb)(WINDOW*,struct Callback,int,int,int)) {
	int p = 0; int sp = 0;
	int y,x; getmaxyx(win,y,x);
	dcb(win, cb, p, 0, y);
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
				return cb.foo[sp](win, data, cb.args[sp]);
			case KEY_UP:
				if (!cb.nmemb||!sp) break;
				if (!p) {
					dcb(win, cb, p, sp, -1);
					wscrl(win, -1);
					wattron(win, COLOR_PAIR(1));
					dcb(win, cb, p, sp-1, -1);
					wattroff(win, COLOR_PAIR(1));
					sp--;
					break;
				}
				dcb(win, cb, p, sp, -1);
				wattron(win, COLOR_PAIR(1));
				dcb(win, cb, p-1, sp-1, -1);
				wattroff(win, COLOR_PAIR(1));
				p--;sp--;
				break;
			case KEY_DOWN:
				if (!cb.nmemb||sp==cb.nmemb-1) break;
				if (p==y-1) {
					dcb(win, cb, p, sp, -1);
					wscrl(win, 1);
					wattron(win, COLOR_PAIR(1));
					dcb(win, cb, p, sp+1, -1);
					wattroff(win, COLOR_PAIR(1));
					sp++;
					break;
				}
				dcb(win, cb, p, sp, -1);
				wattron(win, COLOR_PAIR(1));
				dcb(win, cb, p+1, sp+1, -1);
				wattroff(win, COLOR_PAIR(1));
				p++;sp++;
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
