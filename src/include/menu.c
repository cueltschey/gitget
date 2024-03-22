#include "repos.h"
#include "menu.h"
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

void draw_menu(WINDOW *menu_win, char *options[], int n_options,  int highlight) {
    int x = 2;
    int y = 2;
    werase(menu_win);
    box(menu_win, 0, 0);
    const char* title = "Repos";
    int title_width = strlen(title) + 4; // Add padding for borders
    mvwprintw(menu_win, 0, (getmaxx(menu_win) - title_width) / 2, "[ %s ]", title);
    wprintw(menu_win, "<< prev | next >>");

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
char* user_select_repo(int refresh) {
    initscr();
    curs_set(0);
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    char* repos[200];
    int n_repos;
    if(refresh == 1){
      n_repos = get_repos("cueltschey", repos);
      int fd = open("/tmp/repos", O_WRONLY);
      for(int i = 0; i < n_repos; ++i){
        if(write(fd, repos[i], 100) < 0){
          perror("Write Error");
          exit(EXIT_FAILURE);
        }
      }
    }
    else{
      int fd = open("/tmp/repos", O_RDONLY); 
      if(fd < 0) {
      perror("Open Error");
      exit(EXIT_FAILURE);
      }
      char value[100];
      int bytes_read;
      n_repos = 0;
      while((bytes_read = read(fd, &value, 100)) > 0){
        repos[n_repos] = value;
        n_repos++;
      }
      if(bytes_read < 0){
      perror("Read Error");
      exit(EXIT_FAILURE);
    }
    }

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
    menu_win = newwin(height, width, starty, startx);
    keypad(menu_win, TRUE);
    draw_menu(menu_win, options, n_options, highlight);
    
    while (1) {
        draw_menu(menu_win, options, n_options, highlight);
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
            case 10: // Enter key
                choice = highlight;
                break;
            default:
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
    endwin();
    return options[choice - 1];
}
