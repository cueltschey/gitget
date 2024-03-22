#include "include/menu.h"
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(){
  char* repo = user_select_repo();
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
  }
}
