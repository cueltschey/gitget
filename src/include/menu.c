#include "repos.h"
#include "menu.h"
#include "util.h"
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>


void init_ncurses(){
    initscr();
    curs_set(0);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

void draw_menu(WINDOW *menu_win, char *options[], int n_options,  int highlight, const char* title) {
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    int x = 2;
    int y = 2;
    werase(menu_win);
    box(menu_win, 0, 0);
    int title_width = strlen(title) + 4; // Add padding for borders
    mvwprintw(menu_win, 0, (getmaxx(menu_win) - title_width) / 2, "[ %s ]", title);
    //wprintw(menu_win, "<< prev | next >>");

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

int get_options(char* repos[200], int n_repos, char* options[MAX_OPTIONS], int page){
    int start_index = (page - 1) * 30;
    int end_index = start_index + 30;
    if(end_index > n_repos) end_index = n_repos;

    // Fetch values for the given page
    for (int i = start_index; i < end_index; i++) {
      options[i - start_index] = repos[i];
    }
    return end_index - start_index;
}

int get_options_filtered(char* repos[200], int n_repos, char* options[MAX_OPTIONS], int page, char* filter){
    char** filtered_repos = (char**)malloc(n_repos * sizeof(char*));
    if (filtered_repos == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    int resultSize = 0;
    
    for (int i = 0; i < n_repos; ++i) {
        if(repos[i] == NULL) break;
        //if (strstr(repos[i], filter) != NULL) {
        if(checkSubstring(repos[i], filter) == 1){
            filtered_repos[resultSize] = strdup(repos[i]);
            if (filtered_repos[resultSize] == NULL) {
               fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
            resultSize++;
        }
    }
    int start_index = (page - 1) * 30;
    int end_index = start_index + 30;
    if(end_index > resultSize) end_index = resultSize;

    // Fetch values for the given page
    for (int i = start_index; i < end_index; i++) {
      options[i - start_index] = filtered_repos[i];
    }
    return end_index - start_index;
}

char* user_select_repo(char* token, const char* username) {

    clear();
    refresh();

    char* repos[200];
    int n_repos = get_repos(username, repos, token);

    int choice = 0;
    WINDOW *menu_win;
    int c = 0;
    int page = 1;

    char *options[MAX_OPTIONS];
    int n_options = get_options(repos, n_repos, options, page);
    int highlight = 1;
    int height = n_options + 4; // Adjust the height of the menu window
    int width = 60; // Adjust the width of the menu window
    int starty = (LINES - height) / 2; // Center vertically
    int startx = (COLS - width) / 2; // Center horizontally
    const char* title = "Personal Repos";
    menu_win = newwin(height, width, starty, startx);
    char* search_term = "";
    int search_term_size = 0;
    keypad(menu_win, TRUE);
    while (1) {
        if(search_term_size != 0){
          draw_menu(menu_win, options, n_options, highlight, search_term);
        } else{
          draw_menu(menu_win, options, n_options, highlight, title);
        }
        c = wgetch(menu_win);
        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = n_options;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == n_options)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case KEY_RIGHT:
                if(page * 30 < n_repos){
                  page++; 
                  choice = -1;
                }
                break;
            case KEY_LEFT:
                if(page > 1){
                  choice = -1;
                  page--;
                }
                break;
            case KEY_BACKSPACE:
                if(search_term_size != 0){
                  search_term = popChar(search_term);
                  search_term_size--;
                  page = 1;
                  n_options = get_options_filtered(repos, n_repos, options, page, search_term);
                } 
                if(search_term_size == 0){
                  n_options = get_options(repos, n_repos, options, page);
                }
                break;
            case 10: // Enter key
                choice = highlight;
                break;
            default:
                search_term = appendChar(search_term, c);
                search_term_size++;
                page = 1;
                n_options = get_options_filtered(repos, n_repos, options, page, search_term);
                break;
        }
        if (choice != 0){
          if(choice == -1){
            n_options = get_options(repos, n_repos, options, page);
            highlight = 1;
            choice = 0;
          }
          else break;
        }
    }
    clrtoeol();
    refresh();
    delwin(menu_win);
    return options[choice - 1];
}

char* user_create_repo(int max_length, const char* title) {
    curs_set(1);

    WINDOW *inputwin = newwin(3, max_length + 20, (LINES - 3) / 2, (COLS - max_length - 20) / 2);
    box(inputwin, 0, 0);
    refresh();
    wrefresh(inputwin);

    int title_width = strlen(title) + 4; // Add padding for borders
    mvwprintw(inputwin, 0, (getmaxx(inputwin) - title_width) / 2, "[ %s ]", title);
    keypad(inputwin, TRUE);

    mvwprintw(inputwin, 1, 1, "Enter text: ");
    wrefresh(inputwin);

    char *input = (char *)malloc(max_length + 1);
    memset(input, 0, max_length + 1);
    int ch, i = 0;
    // TODO: fix text jump error
    while ((ch = wgetch(inputwin)) != '\n') {
        if(ch == ' ') continue;
        if (ch == KEY_BACKSPACE && i > 0) {
            mvwprintw(inputwin, 1, i + 13, " ");
            wmove(inputwin, 1, i + 13);
            i--;
        } else if (isprint(ch) && i < max_length) {
            input[i++] = ch;
            mvwprintw(inputwin, 1, i + 13, "%c", ch);
            wmove(inputwin, 1, i + 14);
        }
        wrefresh(inputwin);
    }

    // Clean up
    delwin(inputwin);
    endwin();

    return input;
}

int user_choose_visibility(){
    curs_set(0);

    int highlight = 1;
    int x, y;
    int num_choices = 2;
    const char *choices[] = {
        "Public",
        "Private"
    };

    getmaxyx(stdscr, y, x);

    // Clear the screen
    clear();

    // Create a window for input
    WINDOW *win = newwin(7, x - 4, (y - 7) / 2, 2);
    box(win, 0, 0); // Draw a box around the window
    refresh();
    wrefresh(win); // Refresh the window

    // Print the menu
    mvprintw((y - 7) / 2 + 1, (x - strlen("Choose the visibility of the repository:")) / 2, "Choose the visibility of the repository:");
    refresh();

    // Print options
    for(int i = 0; i < num_choices; i++) {
        if(i == highlight - 1)
            attron(A_REVERSE);
        mvprintw((y - 7) / 2 + 3 + i, (x - strlen(choices[i])) / 2, "%s", choices[i]);
        attroff(A_REVERSE);
    }
    refresh();
    int choice = 0;

    // Loop to navigate through options
    while(1) {
        int c = getch();
        switch(c) {
            case KEY_UP:
                if(highlight == 1)
                    highlight = num_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if(highlight == num_choices)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10: // Enter key pressed
                choice = highlight;
                break;
            default:
                break;
        }
        // Highlight the current choice
        for(int i = 0; i < num_choices; i++) {
            if(i == highlight - 1)
                attron(A_REVERSE);
            mvprintw((y - 7) / 2 + 3 + i, (x - strlen(choices[i])) / 2, "%s", choices[i]);
            attroff(A_REVERSE);
        }
        refresh();
        if(choice != 0) // User made a selection
            break;
    }
    
    delwin(win);
    clear();

    return choice == 1;
}

int user_choose_option(){
    curs_set(0);

    int choice = 0;
    int highlight = 1;
    int x, y;
    int num_choices = 3;
    const char *choices[] = {
        "Clone a personal Repo",
        "Clone any Repo",
        "Create a Repo"
    };

    getmaxyx(stdscr, y, x);

    // Clear the screen
    clear();

    // Create a window for input
    WINDOW *win = newwin(7, x - 4, (y - 7) / 2, 2);
    box(win, 0, 0); // Draw a box around the window
    refresh();
    wrefresh(win); // Refresh the window

    // Print the menu
    mvprintw((y - 7) / 2 + 1, (x - strlen("Choose an Action:")) / 2, "Choose an Action:");
    refresh();

    // Print options
    for(int i = 0; i < num_choices; i++) {
        if(i == highlight - 1)
            attron(A_REVERSE);
        mvprintw((y - 7) / 2 + 3 + i, (x - strlen(choices[i])) / 2, "%s", choices[i]);
        attroff(A_REVERSE);
    }
    refresh();

    // Loop to navigate through options
    while(1) {
        int c = getch();
        switch(c) {
            case KEY_UP:
                if(highlight == 1)
                    highlight = num_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if(highlight == num_choices)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10: // Enter key pressed
                choice = highlight;
                break;
            default:
                break;
        }
        // Highlight the current choice
        for(int i = 0; i < num_choices; i++) {
            if(i == highlight - 1)
                attron(A_REVERSE);
            mvprintw((y - 7) / 2 + 3 + i, (x - strlen(choices[i])) / 2, "%s", choices[i]);
            attroff(A_REVERSE);
        }
        refresh();
        if(choice != 0) // User made a selection
            break;
    }

    delwin(win);
    clear();
    refresh();

    return choice;
}






