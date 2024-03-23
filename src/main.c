#include "include/menu.h"
#include "include/repos.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int clone_no_username(char* repo){
  
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

int clone_username(const char* username, char* repo){

  pid_t pid = fork();

  if (pid < 0) {
      // Fork failed
      perror("fork");
      exit(EXIT_FAILURE);
  } else if (pid == 0) {
      char url[200];
      snprintf(url, sizeof(url),"https://github.com/%s/%s", username, repo);
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
  char* token = getenv("GITHUB_TOKEN");
  if(token == NULL){
    printf("Export your github token to the GITHUB_TOKEN\n");
    exit(EXIT_FAILURE);
  }
  int create_flag = 0;
  int any_flag = 0;
  int personal_flag = 0;
  for(int i = 0; i < argc; ++i){
    if(argv[i][0] != '-') continue;
    if(strchr(argv[i], '-') != NULL && strchr(argv[i], 'c') != NULL){
      create_flag = 1;
    }
    if(strchr(argv[i], '-') != NULL && strchr(argv[i], 'a') != NULL){
      any_flag = 1;
    }
    if(strchr(argv[i], '-') != NULL && strchr(argv[i], 'p') != NULL){
      personal_flag = 1;
    }
  }
  if(create_flag){
    char* new_repo = user_create_repo(30, "Repo Name");
    const char* username = "cueltschey";
    int private = user_choose_visibility();
    create_repo(username, new_repo, private, token);
    clone_username(username, new_repo);
    return 0;
  }
  if(any_flag){
    char* new_repo = user_create_repo(30, "Repo Name");
    char* username = user_create_repo(30, "User Name");
    clone_username(username, new_repo);
    return 0;
  }

  if(personal_flag){
    char* repo = user_select_repo(token);
    if (*repo == '*') {
        memmove(repo, repo + 1, strlen(repo));
    }
    clone_no_username(repo);
    return 0;
  }
  printf("Usage: githelp [flags]\n");
  printf("------------------------\n");
  printf("-a : Clone any repo\n");
  printf("-p : Clone a personal repo\n");
  printf("-c : Create a repo\n");
  printf("\n");
  exit(EXIT_FAILURE);
}

