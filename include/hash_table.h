#ifndef HASH_TABLE
#define HASH_TABLE

#include <stdint.h>

typedef uint32_t (*hash_function)(const char* word);

struct table_t
{
    int size;
    spisok_t** list_arr;
    hash_function func;
};

int    table_ctor (table_t* table, int size, hash_function func);
void   table_dtor (table_t* table);

void   load_table (table_t* table, const char* filename);
bool is_in_list(spisok_t* list, const char* word);
char** read_text  (const char* filename, int* num_lines);

uint32_t one_hash        (const char* word);
uint32_t first_ascii_has (const char* word);
uint32_t lenth_hash      (const char* word);
uint32_t ascii_sum_hash  (const char* word);
uint32_t hash_rol_xor    (const char* word);
uint32_t gnu_hash        (const char* word);
uint32_t crc32c_hash     (const char* word);


#endif