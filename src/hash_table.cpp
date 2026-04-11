#include "hash_table.h"
#include "spisok.h"

int table_ctor(table_t* table, int size, hash_function func)
{
    int err_code = 0;
    int list_size = 10;

    table->size     = size;
    table->list_arr = (spisok_t**)calloc(size, sizeof(spisok_t*));
    for (int i = 0; i < size; i++)
    {
        table->list_arr[i] = (spisok_t*)calloc(1, sizeof(spisok_t));
        err_code += list_ctor(table->list_arr[i], list_size);
    }
    table->func = func;
    
    return err_code;
}

void table_dtor(table_t* table)
{
    for (int i = 0; i < table->size; i++)
    {
        list_dtor(table->list_arr[0]);
    }
    
    free(table->list_arr);
    table->list_arr = NULL;
}

void load_table(table_t* table, const char* filename)
{
    long n_words = 0;
    char** data  = read_text(filename, &n_words);
    
    for (int i = 0; i < n_words; i++)
    {
        long int hash = table->func(data[i]);
        int index = hash % table->size;
        if (table->list_arr[index]->free == 1)
            insert_after(table->list_arr[index], 0, data[i]);
        else if (!is_in_list(table->list_arr[index], data[0]))
            insert_after(table->list_arr[index], 0, data[i]);
    }

    free(data[0]);
    free(data);
}

bool is_in_list(spisok_t* list, const char* word)
{
    int curr_index = list->next[0];

    while (curr_index != 0)
    {
        if (strcmp(list->data[curr_index], word) == 0)
            return true; 

            curr_index = list->next[curr_index];
    }

    return false;
}
