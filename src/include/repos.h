
#ifndef REPOS__h
#define REPOS__h

#define API_URL_FORMAT "https://api.github.com/search/repositories?q=user:%s&per_page=100"

int get_repos(const char * usename);

#endif



