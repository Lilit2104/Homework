#define _GNU_SOURCE
#include "request_response.h"
#include "server_client_signal.h"
#include "config.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "origin.h"
#include "cache.h"


struct cache_entry cache_entries[MAX_CACHE_ENTRIES];
int total_inserted = 0;

int cache_insert(const char* key, char* response, size_t response_len)
{
    int index = total_inserted % MAX_CACHE_ENTRIES;
    size_t key_len = strlen(key);
    if(key_len >= sizeof(cache_entries[index].request_line))
    {
        key_len = sizeof(cache_entries[index].request_line) - 1;
    }
    strncpy(cache_entries[index].request_line, key, key_len);
    cache_entries[index].request_line[key_len] = '\0';
    
    snprintf( cache_entries[index].filename, sizeof(cache_entries[index].filename), "cache_%d.dat", index);

    FILE *fp = fopen(cache_entries[index].filename,"wb");
    if(fp == NULL)
    {
        return -1;
    }
    else
    {
        fwrite(response, 1, response_len, fp);
        fclose(fp);
        cache_entries[index].insertion_order = total_inserted;
        total_inserted++;
        return 0;
    }
}

char* cache_lookup(const char* key)
{
    if (key[0] == '\0') return NULL;

    for(int i = 0; i < MAX_CACHE_ENTRIES; i++)
    {
        if(strcmp(key, cache_entries[i].request_line) == 0)
        {
            return cache_entries[i].filename;
        }
    }
    return NULL;
}

int cache_load(const char *filename, char *dest_buf, size_t *out_len)
{
     FILE *fp1 = fopen(filename,"rb");
     if(fp1 == NULL)
    {
        return -1;
    }
    else
    {
        *out_len = fread(dest_buf, 1, MAX_RESPONSE_SIZE, fp1);
        fclose(fp1);
        
        return 0;
    }
}