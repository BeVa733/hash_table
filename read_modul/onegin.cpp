#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include "hash_table.h"

static const size_t WORD_SIZE = 32;

char** make_ptr_massive(char* buffer, long* num_lines, size_t read_size)
{
    assert(buffer);
    assert(num_lines);

    if (read_size % WORD_SIZE != 0)
    {
        printf("ERROR: binary file has incorrect size\n");
        free(buffer);
        return NULL;
    }

    long word_count = (long)(read_size / WORD_SIZE);

    if (word_count == 0)
    {
        free(buffer);
        return NULL;
    }

    char** words = (char**)calloc((size_t)word_count, sizeof(char*));

    if (!words)
    {
        free(buffer);
        return NULL;
    }

    for (long i = 0; i < word_count; i++)
        words[i] = buffer + i * WORD_SIZE;

    *num_lines = word_count;
    return words;
}

static long int check_file_size(FILE* file)
{
    struct stat file_info = {};
    int fd = fileno(file);
    if (fstat(fd, &file_info)== -1)
    {
        printf("ERROR: check number of words incorrect\n");
        return -1;
    }

    return file_info.st_size;
}

char** read_text(const char* filename, long* num_lines)
{
    assert(filename);
    assert(num_lines);

    FILE* file = fopen(filename, "rb");

    if (file == NULL)
    {
        printf("Reading error\n");
        return NULL;
    }

    long int file_size = check_file_size(file);

    if (file_size <= 0)
    {
        fclose(file);
        return NULL;
    }

    if ((size_t)file_size % WORD_SIZE != 0)
    {
        printf("ERROR: file format is not 32-byte word array\n");
        fclose(file);
        return NULL;
    }

    char* buffer = NULL;

    if (posix_memalign((void**)&buffer, WORD_SIZE, (size_t)file_size) != 0)
    {
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(buffer, sizeof(char), (size_t)file_size, file);
    fclose(file);

    if (read_size != (size_t)file_size)
    {
        free(buffer);
        return NULL;
    }

    return make_ptr_massive(buffer, num_lines, read_size);
}
