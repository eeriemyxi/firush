#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "../include/firush.h"

int main(void) {
  struct ResponseStruct response;
  short errno = 0;

  errno = fir_fetch_html_for("https://nhentai.net/search/?q=english", &response);

  if (errno == 1) {
    fprintf(stderr, "Something went wrong while fetching a URL. Exiting.\n");
    exit(EXIT_FAILURE);
  }

  int* ids = NULL;
  errno = fir_scrape_page_id(response.memory, &ids);
  
  if (errno == 1) {
    fprintf(stderr, "Couldn't compile SCRAPE_ID_RE. Exiting.\n");
    exit(EXIT_FAILURE);
  } else if (errno == 2) {
    fprintf(stderr, "Go to https://downlaodmoreram.com\n");
    exit(EXIT_FAILURE);
  }
  
  for (int i = 0; i < fir_ids_count(&ids); i++) {
    printf("#%d: %d\n", i+1, ids[i]);
  }
  
  int total_pages = 0;
  errno = fir_scrape_total_pages(response.memory, &total_pages);

  if (errno == 1) {
    fprintf(stderr, "Couldn't compile SCRAPE_TOTAL_PAGES. Exiting.\n");
    exit(EXIT_FAILURE);
  } else if (errno == 2) {
    fprintf(stderr, "Go to https://downlaodmoreram.com\n");
    exit(EXIT_FAILURE);
  }
  
  printf("total_pages: %d\n", total_pages);
  
  fir_free_response(&response);
  fir_free_ids(&ids);
  curl_global_cleanup();

  return 0;
}
