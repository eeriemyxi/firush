#define STB_DS_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <curl/curl.h>
#include <regex.h>

#include "../include/stb_ds.h"
#include "../include/firush.h"

#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

static const char *const SCRAPE_TOTAL_PAGE_RE = "page=([0-9]+)\" class=\"last\">";
static const char *const SCRAPE_ID_RE = "href=\"\\/g\\/([0-9]{6})\\/\"";

size_t http_write_callback(void *ptr, size_t size, size_t nmemb, struct ResponseStruct *response) {
  size_t total_size = size * nmemb;

  response->memory = realloc(response->memory, response->size + total_size + 1);

  if (response->memory == NULL) {
    fprintf(stderr, "Buy more RAM from https://downloadmoreram.com.\n");
    exit(1);
  }

  memcpy(&(response->memory[response->size]), ptr, total_size);
  response->size += total_size;
  
  (response->memory)[response->size] = '\0';

  return total_size;
} 

short fir_fetch_html_for(char* url, struct ResponseStruct* response) {
  CURL *curl;
  CURLcode res;
  
  response->memory = malloc(1);
  response->size = 0;
 
  curl_global_init(CURL_GLOBAL_DEFAULT);
 
  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CA_CACHE_TIMEOUT, 604800L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
      curl_easy_cleanup(curl);
      return 1;
    };
    
    curl_easy_cleanup(curl);
    return 0;
  }

  return 2;
}

short fir_scrape_page_id(char* html, int** ids) {
  regex_t     regex;
  regmatch_t  pmatch[2];
  regoff_t    len;

  if (regcomp(&regex, SCRAPE_ID_RE, REG_EXTENDED)) {
    return 1;
  }

  const char *s = html;
  char* res = malloc(sizeof(char*) * (6 + 1));
  
  for (unsigned int i = 0; ; i++) {
    if (regexec(&regex, s, ARRAY_SIZE(pmatch), pmatch, REG_NOTBOL)) {
      break;
    }
    
    len = pmatch[1].rm_eo - pmatch[1].rm_so;

    if (res == NULL) {
      return 2;
    }

    memcpy(res, s + pmatch[1].rm_so, len);
    res[len] = '\0';
    
    arrput(*ids, strtol(res, NULL, 10));
    
    s += pmatch[0].rm_eo;
  }
  
  free(res);

  return 0;
}

short fir_scrape_total_pages(char* html, int* total_pages) {
  regex_t     regex;
  regmatch_t  pmatch[2];
  regoff_t    len;

  if (regcomp(&regex, SCRAPE_TOTAL_PAGE_RE, REG_EXTENDED)) {
    return 1;
  }

  const char *s = html;
  
  if (regexec(&regex, s, ARRAY_SIZE(pmatch), pmatch, REG_NOTBOL)) {
    return 0;
  }
  
  len = pmatch[1].rm_eo - pmatch[1].rm_so;

  char* res = malloc(sizeof(char*) * len + 1);

  if (res == NULL) {
    free(res);
    return 2;
  }
  
  memcpy(res, s + pmatch[1].rm_so, len);
  res[len] = '\0';
  
  *total_pages = strtol(res, NULL, 10);
  
  free(res);
  return 0;
}

size_t fir_ids_count(int** ids) {
  return arrlenu(*ids);
}

void fir_free_response(struct ResponseStruct* resp) {
  free(resp->memory);
}

void fir_free_ids(int** ids) {
  arrfree(*ids);
}
