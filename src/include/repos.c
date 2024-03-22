#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "util.h"
#include "cJSON.h"
#include "repos.h"


int get_repos(const char* username, char* repos[30], int page){
    // Initialize libcurl
    CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;
    curl = curl_easy_init();
    if (!curl) {
        perror("Libcurl Init Error");
        return EXIT_FAILURE;
    }

    // Create API URL
    char* token = "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";
    char api_url[256];
    char auth[256];
    snprintf(api_url, sizeof(api_url), API_URL_FORMAT, username, page);
    snprintf(auth, sizeof(auth), "Authorization: Bearer %s", token);
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, auth);

    curl_easy_setopt(curl, CURLOPT_URL, api_url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Perform the request
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Error: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return EXIT_FAILURE;
    }

    cJSON *root = cJSON_Parse(chunk.memory);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    int index = 0;

    if (items != NULL && cJSON_IsArray(items)) {
        cJSON *element = NULL;
        cJSON_ArrayForEach(element, items) {
            // TODO: fix bug with private repos
            cJSON *full_name = cJSON_GetObjectItem(element, "full_name");
            cJSON *private = cJSON_GetObjectItem(element, "private");

            if (full_name != NULL) {
                if(private != NULL && cJSON_IsTrue(private)){
                  char *colored_name = malloc(strlen(full_name->valuestring) + 6);
                  if (colored_name != NULL) {
                      sprintf(colored_name, "%s-(p) ", full_name->valuestring);
                      repos[index] = colored_name;
                      index++;
                  }
                }
                else{
                  char* regular_name = malloc(strlen(full_name->valuestring));
                  if(regular_name != NULL){
                      sprintf(regular_name, "%s ", full_name->valuestring);
                      repos[index] = regular_name;
                      index++;
                  }
                }
            }
        }
        cJSON_Delete(root);
    }
    if(index == 29){
      char* next = "NEXT =>";
      repos[index] = next;
    }
    // Cleanup
    curl_easy_cleanup(curl);
    free(chunk.memory);
    return index + 1;
}

