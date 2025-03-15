#include <stdio.h>
#include <stdlib.h>

struct ResponseStruct {
  char* memory;
  int size;
};

short fir_fetch_html_for(char* url, struct ResponseStruct* response) ;

short fir_scrape_page_id(char* html, int** ids);

short fir_scrape_total_pages(char* html, int* total_pages);

size_t fir_ids_count(int** ids);

void fir_free_response(struct ResponseStruct* resp);

void fir_free_ids(int** ids);
