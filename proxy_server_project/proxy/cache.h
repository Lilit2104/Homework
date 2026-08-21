#ifndef CACHE_H
#define CACHE_H

#include "config.h"
#include <unistd.h>

struct cache_entry {
    char request_line[256];
    char filename[256];
    unsigned long insertion_order;
};

char* cache_lookup(const char* key);
int cache_insert(const char* key, char* response,size_t response_len);
int cache_load(const char *filename, char *dest_buf, size_t *out_len);

#endif