
#ifndef REPOS__h
#define REPOS__h

#define API_URL_FORMAT "https://api.github.com/search/repositories?q=user:%s&per_page=200"

int get_repos(const char * usename, char* repos[200], char* token);

int create_repo(const char *username, const char *repo_name, int is_private, char* token);

#endif



