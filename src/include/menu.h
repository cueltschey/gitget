#include <ncurses.h>

#ifndef MENU__h
#define MENU__h


#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define MAX_OPTIONS 31



void draw_menu(WINDOW *menu_win, char *options[], int n_options,  int highlight);

int get_options(char* repos[200], int n_repos, char* options[MAX_OPTIONS], int page);

void filter_search_terms(char *search_terms[], int num_terms);

char* user_select_repo();

#endif
