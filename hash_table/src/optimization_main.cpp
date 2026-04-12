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
    if (argc < 4)
    {
        printf("Usage: %s <table_text.txt> <test_text.txt> <table_size>\n", argv[0]);
        return 1;
    }

    const char* table_filename = argv[1];
    const char* test_filename = argv[2];
    int table_size = atoi(argv[3]);

    if (table_size <= 0)
        return 1;

    table_t table = {};

    if (table_ctor(&table, table_size, crc32c_hash) != 0)
        return 1;

    char** table_data = load_table(&table, table_filename);
    if (!table_data)
    {
        table_dtor(&table);
        return 1;
    }

    printf("Table loaded\n");

    long n_test_words = 0;
    char** test_data = read_text(test_filename, &n_test_words);

    if (test_data == NULL)
    {
        table_dtor(&table);
        free_text_data(table_data);
        return 1;
    }

    volatile unsigned long long found_count = 0;
    volatile unsigned long long miss_count  = 0;

    for (int i = 0; i < n_test_words; i++)
    {
        if (find_in_table(&table, test_data[i]) != NULL)
            found_count++;
        else
            miss_count++;
    }

    table_dtor(&table);
    free_text_data(table_data);
    free_text_data(test_data);

    printf("Done\n");

    return 0;
}