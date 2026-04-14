#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "hash_table.h"

static void free_text_data(char** data)
{
    if (data == NULL)
        return;

    free(data[0]);
    free(data);
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <table_text.txt> <query_text.txt> [table_size]\n", argv[0]);
        return 1;
    }
    int table_size = 0;

    if(!argv[3])
        table_size = 11717;
    else
        table_size = atoi(argv[3]);

    const char* table_filename = argv[1];
    const char* query_filename = argv[2];

    if (table_size <= 0)
        return 1;

    table_t table = {};

    if (table_ctor(&table, table_size, crc32c_hash) != 0)
        return 1;

    char** table_data = load_table(&table, table_filename);

    if (table_data == NULL)
    {
        table_dtor(&table);
        return 1;
    }

    long n_query_words = 0;
    char** query_data = read_text(query_filename, &n_query_words);

    if (query_data == NULL)
    {
        table_dtor(&table);
        free_text_data(table_data);
        return 1;
    }

    volatile unsigned long long found_count = 0;
    volatile unsigned long long miss_count  = 0;

    for (int i = 0; i < n_query_words; i++)
    {
        if (query_data[i] == NULL || query_data[i][0] == '\0')
            continue;

        if (find_in_table(&table, query_data[i]) != NULL)
            found_count++;
        else
            miss_count++;
    }

    table_dtor(&table);
    free_text_data(table_data);
    free_text_data(query_data);

    return 0;
}