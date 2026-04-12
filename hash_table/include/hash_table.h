#ifndef HASH_TABLE
#define HASH_TABLE

#include "spisok.h"
#include <stdint.h>

typedef uint32_t (*hash_function)(const char* word);

struct table_t
{
    int size;
    spisok_t** list_arr;
    hash_function func;
};

// table API
int    table_ctor     (table_t* table, int size, hash_function func);
void   table_dtor     (table_t* table);
bool   is_in_list     (spisok_t* list, const char* word, int* find_index);
char** load_table     (table_t* table, const char* filename);
bool insert_in_table  (table_t* table, const char* word);
bool is_in_table      (const table_t* table, const char* word);
char* find_in_table   (const table_t* table, const char* word);
bool erase_from_table (table_t* table, const char* word);

// utils
char** read_text  (const char* filename, long* num_lines);
int get_table_index(const table_t* table, const char* word);

// statistics funcs
int empty_bucket_count   (const table_t* table);
int max_bucket_size      (const table_t* table);
double table_load_factor (const table_t* table);
int  bucket_size         (const spisok_t* list);
int  table_elem_count    (const table_t* table);
void dump_table_hist     (const table_t* table, const char* filename);

// hash funcs
uint32_t one_hash        (const char* word);
uint32_t first_ascii_hash(const char* word);
uint32_t lenth_hash      (const char* word);
uint32_t ascii_sum_hash  (const char* word);
uint32_t hash_rol_xor    (const char* word);
uint32_t gnu_hash        (const char* word);
uint32_t crc32c_hash     (const char* word);

#endif