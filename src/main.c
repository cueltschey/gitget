#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define API_URL_FORMAT "https://api.github.com/users/%s/repos"

// Callback function to write received data into a string
size_t write_callback(void *ptr, size_t size, size_t nmemb, char *data) {
    size_t total_size = size * nmemb;
    strncat(data, (char *)ptr, total_size);
    return total_size;
}


int main(int argc, char** argv){
  if(argc > 1){
    printf("%s", argv[1]);
  }
  if (argc != 2) {
        fprintf(stderr, "Usage: %s <github_username>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Initialize libcurl
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Error initializing libcurl\n");
        return EXIT_FAILURE;
    }

    // Create API URL
    char api_url[256];
    snprintf(api_url, sizeof(api_url), API_URL_FORMAT, argv[1]);

    // Create buffer to store response
    char response_buffer[4096] = {0};

    // Set curl options
    curl_easy_setopt(curl, CURLOPT_URL, api_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    // Perform the request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Error: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return EXIT_FAILURE;
    }

    // Print the response
    printf("Repositories for user '%s':\n", argv[1]);
    printf("%s\n", response_buffer);

    // Cleanup
    curl_easy_cleanup(curl);
    return EXIT_SUCCESS;
}

