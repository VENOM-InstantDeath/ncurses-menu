#include <ncurses.h>
struct Data {
};
struct Callback {
	int (**foo)(WINDOW*, struct Data*, void*);
	void** args;
	char** ls;
	int nmemb;
};

void display(WINDOW* win, struct Callback cb, int p, int start, int stop);
int menu(WINDOW* win, struct Data* data, struct Callback cb, void (*dcb)(WINDOW*,struct Callback,int,int,int));
