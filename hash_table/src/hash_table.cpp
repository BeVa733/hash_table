#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hash_table.h"

// --------------------- Table API ----------------------------

int table_ctor(table_t* table, uint32_t size, hash_function func)
{
    int err_code = 0;
    int list_size = 2;

    table->size     = (uint64_t)size;

    table->list_arr = (spisok_t**)calloc(size, sizeof(spisok_t*));
    for (uint32_t i = 0; i < size; i++)
    {
        table->list_arr[i] = (spisok_t*)calloc(1, sizeof(spisok_t));
        err_code += list_ctor(table->list_arr[i], list_size);
    }

    table->func = func;

    table->magic_const = (UINT64_MAX / (uint32_t)size) + 1;

    return err_code;
}

void table_dtor(table_t* table)
{
    for (uint64_t i = 0; i < table->size; i++)
    {
        list_dtor(table->list_arr[i]);
        free(table->list_arr[i]);
    }
    
    free(table->list_arr);
    table->list_arr = NULL;
}

bool is_in_list(spisok_t* list, const char* word, int* find_index = NULL)
{
    int curr_index = list->next[0];

    while (curr_index != 0)
    {
        if (*(list->data[curr_index]) == *word)
        {

            if (strcmp(list->data[curr_index], word) == 0)
            {
                if (find_index)
                    *find_index = curr_index;

                return true; 
            }
        }

        curr_index = list->next[curr_index];
    }

    return false;
}

char** load_table(table_t* table, const char* filename)
{
    assert(table);   
    assert(filename);

    long n_words = 0;
    char** data  = read_text(filename, &n_words);

    if (data == NULL)
        return NULL;
    
    for (int i = 0; i < n_words; i++)
        insert_in_table(table, data[i]);

    return data;
}

bool insert_in_table(table_t* table, const char* word)
{
    assert(table);
    assert(word);

    int index = get_table_index(table, word);

    if (table->list_arr[index]->next[0] == 0)
    {
        insert_after(table->list_arr[index], 0, word);
        return true;
    }

    if (!is_in_list(table->list_arr[index], word))
    {
        insert_after(table->list_arr[index], 0, word);
        return true;
    }

    return false;
}

bool is_in_table(const table_t* table, const char* word)
{
    assert(table);
    assert(word);

    int index = get_table_index(table, word);

    return is_in_list(table->list_arr[index], word);
}

// const char* find_in_table(const table_t* table, const char* word)
// {
//     assert(table);
//     assert(word);

//     uint32_t hash = table->func(word);

//     uint32_t index = 0;
//     uint64_t magic_const = table->magic_const;
//     uint32_t size = table->size;

//     asm volatile (
//     ".intel_syntax noprefix  \n\t"

//     "mul %[sz]               \n\t" 
//     "mul %[mc]               \n\t" 

//     ".att_syntax prefix      \n\t"
//     : [index] "=d" (index)         
//     : [mc]    "r"  (magic_const),  
//       [hash]  "a"  (hash),          
//       [sz]    "r"  (size)          
//     : "cc"                  
//     );

//     int find_index = 0;

//     if (is_in_list(table->list_arr[index], word, &find_index))
//         return table->list_arr[index]->data[find_index];

//     return NULL;
// }

bool erase_from_table(table_t* table, const char* word)
{
    assert(table);
    assert(word); 

    int index = get_table_index(table, word);

    int find_index = 0;

    if (!is_in_list(table->list_arr[index], word, &find_index))
        return false;

    list_delete(table->list_arr[index], find_index);

    return true;
}


// ---------------- Utils -----------------------


int get_table_index(const table_t* table, const char* word)
{
    assert(table);
    assert(word);

    uint32_t hash = table->func(word);
    int index = hash % table->size;

    return index;
}


// ---------------- Statistic funcs ----------------

int empty_bucket_count(const table_t* table)
{
    assert(table);

    int count = 0;

    for (uint64_t i = 0; i < table->size; i++)
    {
        if (bucket_size(table->list_arr[i]) == 0)
            count++;
    }

    return count;
}

int max_bucket_size(const table_t* table)
{
    assert(table);

    int max_size = 0;

    for (uint64_t i = 0; i < table->size; i++)
    {
        int curr_size = bucket_size(table->list_arr[i]);

        if (curr_size > max_size)
            max_size = curr_size;
    }

    return max_size;
}

double table_load_factor(const table_t* table)
{
    assert(table);

    return (double)table_elem_count(table) / (double)table->size;
}

int bucket_size(const spisok_t* list)
{
    assert(list);

    int count = 0;

    for (int curr_index = list->next[0]; curr_index != 0; curr_index = list->next[curr_index])
        count++;

    return count;
}

int table_elem_count(const table_t* table)
{
    assert(table);

    int count = 0;

    for (uint64_t i = 0; i < table->size; i++)
        count += bucket_size(table->list_arr[i]);

    return count;
}

void dump_table_hist(const table_t* table, const char* filename)
{
    assert(table);
    assert(filename);

    FILE* file = fopen(filename, "w");
    if (file == NULL)
        return;

    fprintf(file, "bucket,size\n");

    for (uint64_t i = 0; i < table->size; i++)
        fprintf(file, "%ld,%d\n", i, bucket_size(table->list_arr[i]));

    fclose(file);
}