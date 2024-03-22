
#ifndef REPOS__h
#define REPOS__h

#define API_URL_FORMAT "https://api.github.com/search/repositories?q=user:%s&per_page=29&page=%d"

int get_repos(const char * usename, char* repos[30], int page);

#endif



