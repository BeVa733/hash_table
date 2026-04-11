#ifdef _WIN32
#include <TXLib.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>

#ifdef _WIN32
#include <Windows.h>
#endif

//#define FINAL_VER //(для отключения верификации)

#include "spisok.h"


int list_ctor(spisok_t* list, int capacity)
{
    assert(list);

    list->data = (list_t*)calloc(capacity, sizeof(list->data[0])); 
    list->next = (int*)calloc(capacity, sizeof(list->next[0]));
    list->prev = (int*)calloc(capacity, sizeof(list->prev[0]));
    list->capacity = capacity;
    if (!(list->data && list->next && list->prev))
    {
        printf("Memory allocation error\n");
        return 1;
    }


    for(int i = 0; i < capacity; i++)
    {
        list->data[i] = LIST_POISON_VALUE;
        list->next[i] = i + 1;
        list->prev[i] = POIZON;
    }
    list->next[capacity - 1] = POIZON;
    list->next[0] = 0;
    list->prev[0] = 0;

    list->free = 1;

    ON_NOT_FINAL(LIST_VERIFY_BEFORE)

    return 0;
}

int list_realloc(spisok_t* list)
{
    assert(list);

    int new_capacity = list->capacity * 2;

    list->data = (list_t*)realloc(list->data, new_capacity * sizeof(list->data[0]));
    list->next = (int*)realloc(list->next, new_capacity * sizeof(list->next[0]));
    list->prev = (int*)realloc(list->prev, new_capacity * sizeof(list->prev[0]));

    for (int i = list->capacity; i < new_capacity; i++)
    {
        list->data[i] = LIST_POISON_VALUE;
        list->prev[i] = POIZON;
        list->next[i] = i + 1;
    }

    list->next[new_capacity - 1] = POIZON;

    list->free = list->capacity;

    list->capacity = new_capacity;

    ON_NOT_FINAL(LIST_VERIFY_AFTER)

    return 0;
}

int list_realloc_down_soft(spisok_t* list)
{
    assert(list);

    ON_NOT_FINAL(LIST_VERIFY_BEFORE)

    int max_used_index = 0;
    for (int i = 0; i < list->capacity; i++)
    {
        if (list->prev[i] != POIZON && i > max_used_index)
            max_used_index = i;

    }

    int new_capacity = max_used_index + 1;

    if (new_capacity >= list->capacity)
        return SUCCESS;

    list->capacity = new_capacity;

    list->free = POIZON;
    for (int i = new_capacity - 1; i >= 0; i--)
    {
        if (list->prev[i] == POIZON && i != 0)
        {
            list->next[i] = list->free;
            list->free = i;
        }
    }

    ON_NOT_FINAL(LIST_VERIFY_AFTER)

    return SUCCESS;
}

void list_dtor(spisok_t* list)
{
    free(list->data);
    free(list->next);
    free(list->prev);

    list->data = NULL;
    list->next = NULL;
    list->prev = NULL;

    list->capacity = 0;
    list->free = 0;
}

int list_print(spisok_t* list)
{
    assert(list);

    ON_NOT_FINAL(LIST_VERIFY_BEFORE)

    for(int i = 1; i != 0; i = list->next[i])
        printf("%" PRINT_SPEC " -> ", list->data[i]);

    printf("END");

    ON_NOT_FINAL(LIST_VERIFY_AFTER)

    return 0;
}

int insert_after(spisok_t* list, int index, list_t value)
{
    assert(list);

    ON_NOT_FINAL(LIST_VERIFY_BEFORE)

    if (list->prev[index] == POIZON || index == POIZON)
    {
        printf("ERROR: CANNOT INSERT (BAD ANHCOR)");
        return POIZON;
    }

    if (list->free == POIZON)
        list_realloc(list);


    int new_index = list->free;
    list->free = list->next[new_index];

    list->data[new_index] = value;
    list->prev[new_index] = index;
    list->next[new_index] = list->next[index];

    list->prev[list->next[index]] = new_index;
    list->next[index] = new_index;

    ON_NOT_FINAL(LIST_VERIFY_AFTER)

    return new_index;
}

int insert_before(spisok_t* list, int index, list_t value)
{
    assert(list);

    return insert_after(list, list->prev[index], value);
}

int list_delete (spisok_t* list, int index)
{
    assert(list);

    ON_NOT_FINAL(LIST_VERIFY_BEFORE)

    list->next[list->prev[index]] = list->next[index];
    list->prev[list->next[index]] = list->prev[index];

    list->data[index] = LIST_POISON_VALUE; 
    list->prev[index] = POIZON;

    list->next[index] = list->free;
    list->free = index;

    ON_NOT_FINAL(LIST_VERIFY_AFTER)

    return SUCCESS;
}

int list_sort(spisok_t* list)
{
    assert(list);

    ON_NOT_FINAL(LIST_VERIFY_BEFORE)

    list_t* data_copy_arr = (list_t*)calloc(list->capacity, sizeof(list_t));
    if (data_copy_arr == NULL)
    {
        printf("Memory allocation failed in list_sort\n");
        return POIZON;
    }

    int n_data = 0;
    for(int i = 1; i != 0; i = list->next[i])
        data_copy_arr[n_data++] = list->data[i];

    for (int i = 0; i < n_data; i++)
    {
        list->data[i+1] = data_copy_arr[i];
        list->next[i+1] = i + 2;
        list->prev[i+1] = i;
    }
    list->next[n_data] = 0;
    list->next[0] = 1;
    list->prev[0] = n_data;

    if (n_data + 1 <= list->capacity)
    {
        list->free = n_data + 1;

        for(int i = n_data + 1; i < list->capacity; i++)
        {
            list->data[i] = LIST_POISON_VALUE;
            list->prev[i] = POIZON;
            list->next[i] = i + 1;
        }

        list->next[list->capacity - 1] = POIZON;
    }

    else
        list->free = POIZON;

    free(data_copy_arr);

    ON_NOT_FINAL(LIST_VERIFY_AFTER)

    return 0;
}

int list_realloc_down_hard(spisok_t* list)
{
    assert(list);

    ON_NOT_FINAL(LIST_VERIFY_BEFORE)

    list_sort(list);
    list_realloc_down_soft(list);

    ON_NOT_FINAL(LIST_VERIFY_AFTER)

    return 0;
}

#ifdef _WIN32
void check_time(spisok_t* list)
{
    LARGE_INTEGER start = {};
    LARGE_INTEGER end = {};

    QueryPerformanceCounter(&start);

    int curr_ind = 0;
    for (int i = 0; i < 10e7; i++)
    {
        curr_ind = insert_after(list, 0, 20);
        list_delete(list, curr_ind);
    }

    QueryPerformanceCounter(&end);

    double ticks = (double)(end.QuadPart - start.QuadPart);

    printf("Ticks: %.0lf\n", ticks);
}
#endif
