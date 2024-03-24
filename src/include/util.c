#include "util.h"

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    printf("error: not enough memory\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

char* appendChar(char* str, char c) {
    // Find the length of the original string
    size_t len = strlen(str);
    
    // Allocate memory for the new string (original string length + 1 for the new character + 1 for the null terminator)
    char* newStr = (char*)malloc((len + 2) * sizeof(char));
    
    // Check if memory allocation was successful
    if (newStr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    // Copy the original string into the new memory
    strcpy(newStr, str);
    
    // Append the new character
    newStr[len] = c;
    
    // Add the null terminator
    newStr[len + 1] = '\0';
    
    return newStr;
}

char* popChar(char* str) {
    // Find the length of the original string
    size_t len = strlen(str);

    // Allocate memory for the new string (original string length - 1 for removing the last character + 1 for the null terminator)
    char* newStr = (char*)malloc(len * sizeof(char));
    
    // Check if memory allocation was successful
    if (newStr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    // Copy the original string into the new memory except for the last character
    strncpy(newStr, str, len - 1);

    // Add the null terminator
    newStr[len - 1] = '\0';
    
    return newStr;
}


int checkSubstring(char* str, char* sub){
  char *p1, *p2, *p3;
  int i=0,j=0,flag=0;

  p1 = str;
  p2 = sub;

  for(i = 0; i<strlen(str); i++)
  {
    if(*p1 == *p2)
      {
          p3 = p1;
          for(j = 0;j<strlen(sub);j++)
          {
            if(*p3 == *p2)
            {
              p3++;p2++;
            } 
            else
              break;
          }
          p2 = sub;
          if(j == strlen(sub))
          {
             flag = 1;
          }
      }
    p1++; 
  }
  return flag;
}

