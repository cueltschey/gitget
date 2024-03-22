#include "include/repos.h"
#include <ncurses.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define MAX_OPTIONS 31

void draw_menu(WINDOW *menu_win, char *options[], int n_options,  int highlight) {
    int x = 2;
    int y = 2;
    werase(menu_win);
    box(menu_win, 0, 0);
    int lines;
    if(n_options >= 30){
      lines = n_options;
    }
    else{
      lines = n_options - 1;
    }  
    for (int i = 0; i < lines; ++i) {
        if (highlight == i + 1) {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", options[i]);
            wattroff(menu_win, A_REVERSE);
        } else {
            mvwprintw(menu_win, y, x, "%s", options[i]);
        }
        ++y;
    }
    wrefresh(menu_win);
}

int get_options(const char* username, char* options[MAX_OPTIONS], int page){
    int n_options = get_repos("cueltschey", options, page);
    int index = n_options;
    while(index < MAX_OPTIONS){
      options[index] = NULL;
      index++;
    }
    return n_options;
}

int main() {
    initscr();
    curs_set(0);
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    int choice = 0;
    char *options[MAX_OPTIONS];
    WINDOW *menu_win;
    int c = 0;
    int page = 1;
    int height, width, startx, starty;
    int highlight = 1;

    int n_options = get_options("cueltschey", options, page);
    highlight = 1;
    height = n_options + 4; // Adjust the height of the menu window
    width = 60; // Adjust the width of the menu window
    starty = (LINES - height) / 2; // Center vertically
    startx = (COLS - width) / 2; // Center horizontally
    menu_win = newwin(height, width, starty, startx);
    keypad(menu_win, TRUE);
    
    draw_menu(menu_win, options, n_options, highlight);

    while (1) {
        c = wgetch(menu_win);
        switch (c) {
            case KEY_UP:
                if (highlight == 1 && n_options == 30)
                    highlight = n_options;
                else if (highlight == 1)
                    highlight = n_options - 1;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == n_options && n_options == 30)
                    highlight = 1;
                else if (highlight == n_options - 1 && n_options < 30)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10: // Enter key
                choice = highlight;
                break;
            default:
                break;
        }
        if (choice != 0){
          if(choice == 30){
            page++;
            n_options = get_options("cueltschey", options, page);
            highlight = 1;
            choice = 0;
          }
          else break;
        }
        draw_menu(menu_win, options, n_options, highlight);
    }

    mvprintw(LINES - 2, 0, "You chose option %d with a choice of %s", choice, options[choice - 1]);
    clrtoeol();
    refresh();
    getch();
    endwin();

    return 0;
}
