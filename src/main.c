#include "include/menu.h"
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int clone(int refresh){
  char* repo = user_select_repo(refresh);
  printf("%s\n", repo);
  if (*repo == '*') {
      memmove(repo, repo + 1, strlen(repo));
  }
  pid_t pid = fork();

  if (pid < 0) {
      // Fork failed
      perror("fork");
      exit(EXIT_FAILURE);
  } else if (pid == 0) {
      char url[200];
      snprintf(url, sizeof(url),"https://github.com/%s", repo);
      execl("/usr/bin/git", "git", "clone", url, NULL);
      perror("execl");
      exit(EXIT_FAILURE);
  } else {
      int status;
      // Wait for the child process to finish
      waitpid(pid, &status, 0);
      return WEXITSTATUS(status);
  }

}

int main(int argc, char** argv){
  int refresh_flag = 0;
  for(int i = 0; i < argc; ++i){
    if(strchr(argv[i], '-') != NULL && strchr(argv[i], 'r') != NULL){
      refresh_flag = 1;
    } 
  }
  clone(refresh_flag);
}
