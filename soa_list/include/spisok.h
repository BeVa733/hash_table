#ifndef SPISOK_H
#define SPISOK_H

const int POIZON  = -1;
const int SUCCESS =  0;

#ifndef LIST_TYPE
#define LIST_TYPE int 
#endif

#ifndef PRINT_SPEC
#define PRINT_SPEC "d"
#endif

typedef LIST_TYPE list_t;

#ifndef LIST_POISON_VALUE
#define LIST_POISON_VALUE ((list_t)0)
#endif

struct spisok_t
{
    list_t* data;
    int* next;
    int* prev;
    int free;
    int capacity;
};

enum list_verif_error
{
    LIST_NO_ERRORS         = 0b00000000,
    LIST_BAD_MAIN_PTR      = 0b10000000,
    LIST_BAD_DATA_PTR      = 0b01000000,
    LIST_BAD_NEXT_PTR      = 0b00100000,
    LIST_BAD_PREV_PTR      = 0b00010000,
    LIST_BAD_ANCHOR_ELEM   = 0b00001000,
    LIST_NEXT_CYCLED       = 0b00000100,
    LIST_PREV_CYCLED       = 0b00000010,
    LIST_NEXT_NOT_EQV_PREV = 0b00000001
};

struct graph_call_info
{
    const char* filename;
    const char* function;
    int line;
    const char* call_position;
    spisok_t* list_cpy;
};

const int MAX_GRAPH_CALLS = 100;

extern graph_call_info graph_calls[MAX_GRAPH_CALLS];
extern int graph_count;

#ifdef DUMP_CPP
    unsigned int list_verif_code = 0;
#else
    extern unsigned int list_verif_code;
#endif

#define LIST_VERIFY_HELPER(position)                                                    \
        list_verif_code = list_verif(list);                                             \
        if (graph_count < MAX_GRAPH_CALLS)                                              \
        {                                                                               \
            graph_calls[graph_count].filename = __FILE__;                               \
            graph_calls[graph_count].function = __func__;                               \
            graph_calls[graph_count].line = __LINE__;                                   \
            graph_calls[graph_count].call_position = position;                          \
            if (graph_calls[graph_count].list_cpy == NULL)                              \
                graph_calls[graph_count].list_cpy = (spisok_t*)calloc(1, sizeof(spisok_t)); \
            list_info_copy(list, graph_calls[graph_count].list_cpy);                    \
        }                                                                               \
        list_graph (list, "test1.txt");                                                 \
        if(list_verif_code != LIST_NO_ERRORS)                                           \
        {                                                                               \
            list_dump(list_verif_code);                                                 \
            return POIZON;                                                              \
        }


#define LIST_VERIFY_BEFORE LIST_VERIFY_HELPER("BEFORE")
#define LIST_VERIFY_AFTER LIST_VERIFY_HELPER("AFTER")

#ifndef FINAL_VER
#define ON_NOT_FINAL(code) code
#else
#define ON_NOT_FINAL(code)
#endif

void list_graph  (spisok_t* list, const char* filename);
void make_table  (spisok_t* list, FILE* file, int index, const char* type);
void make_order  (spisok_t* list, FILE* file);
void make_arrows (const spisok_t* list, FILE* file);
void make_graph  (const char* filename);

unsigned int list_verif (const spisok_t* list);
void list_dump          (unsigned int error_code);

void list_info_copy   (spisok_t* list, spisok_t* list_cpy);
void graph_calls_dtor (void);

int list_ctor    (spisok_t* list, int size);
int list_realloc (spisok_t* list);
void list_dtor   (spisok_t* list);

int list_realloc_down_soft (spisok_t* list);
int list_realloc_down_hard (spisok_t* list);

int insert_after  (spisok_t* list, int index, const list_t value);
int insert_before (spisok_t* list, int index, const list_t value);
int list_delete   (spisok_t* list, int index);
int list_sort     (spisok_t* list);
int list_print    (spisok_t* list);

#ifdef _WIN32
void check_time(spisok_t* list);
#endif

#endif
