#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>

#include "hash_table.h"

static inline bool is_sep(char symbol)
{
    return symbol == ' ' || symbol == '\n' || symbol == '\t';
}

static char** make_ptr_massive(char* buffer, long* num_lines, size_t read_size)
{
    assert(buffer != NULL);
    assert(num_lines != NULL);

    long capacity = read_size >> 3;
    char** lines = (char**)calloc(capacity, sizeof(char*));
    if (!lines)
    {
        free(buffer);
        return NULL;
    }

    long line_index = 0;
    bool inside_word = false;

    for (size_t i = 0; i < read_size; i++)
    {
        if (is_sep(buffer[i]))
        {
            buffer[i] = '\0';
            inside_word = false;
        }
        else if (!inside_word)
        {
            if (line_index >= capacity)
            {
                capacity *= 2;
                char** new_lines = (char**)realloc(lines, capacity * sizeof(char*));
                if (!new_lines)
                {
                    free(lines);
                    free(buffer);
                    return NULL;
                }

                lines = new_lines;
            }

            lines[line_index++] = &buffer[i];
            inside_word = true;
        }
    }

    *num_lines = line_index;

    if (*num_lines == 0)
    {
        free(lines);
        free(buffer);
        return NULL;
    }

    return lines;
}

static long int check_file_size(FILE* file)
{
    struct stat file_info = {};
    int fd = fileno(file);
    if (fstat(fd, &file_info)== -1)
    {
        printf("ERROR: check number of lines incorrect\n");
        return -1;
    }

    return file_info.st_size;
}

char** read_text(const char* filename, long* num_lines)
{
    assert(filename != NULL); 
    assert(num_lines != NULL); 

    FILE* file = fopen(filename, "r");
    if (!file)
    {
        printf("Reading error\n");
        return 0;
    }

    long int file_size = check_file_size(file);
    if (file_size < 0)
    {
        fclose(file);
        return NULL;
    }

    char* buffer = (char*)calloc(file_size + 1, sizeof(char));
    if (!buffer)
    {
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(buffer, sizeof(char), file_size, file);

    fclose(file);

    buffer[read_size] = '\0';

    return make_ptr_massive(buffer, num_lines, read_size);
}