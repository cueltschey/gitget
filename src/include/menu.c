#include "repos.h"
#include "menu.h"

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
            if(options[i][0] == '*'){
              attron(COLOR_PAIR(2));
              mvwprintw(menu_win, y, x, "%s", options[i]);
              attroff(COLOR_PAIR(2));
            }
            else{
              mvwprintw(menu_win, y, x, "%s", options[i]);
            }
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

void filter_search_terms(char *search_terms[], int num_terms) {
    if (search_terms == NULL || num_terms <= 0) {
        return;
    }

    for (int i = 0; i < num_terms; i++) {
        if (search_terms[i] != NULL) {
            size_t length = strlen(search_terms[i]);
            size_t start = 0;

            // Remove leading whitespace
            while (start < length && isspace(search_terms[i][start])) {
                start++;
            }

            // Remove trailing whitespace
            size_t end = length;
            while (end > start && isspace(search_terms[i][end - 1])) {
                end--;
            }

            // Shift characters to the beginning of the string
            memmove(search_terms[i], search_terms[i] + start, end - start);
            search_terms[i][end - start] = '\0'; // Null-terminate the string
        }
    }
}


// TODO: implement previous page
char* user_select_repo() {
    initscr();
    curs_set(0);
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);

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
            case KEY_RIGHT:
                if(n_options == 30)
                  choice = 30;
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
    clrtoeol();
    refresh();
    delwin(menu_win);
    endwin();
    return options[choice - 1];
}
