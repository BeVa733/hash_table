#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <immintrin.h>

#include "hash_table.h"

char** make_ptr_massive(char* buffer, long* num_lines, size_t read_size)
{
    assert(buffer != NULL);
    assert(num_lines != NULL);

    long capacity = (read_size >> 1) + 1;
    char** words = (char**)calloc(capacity, sizeof(char*));
    if (!words)
    {
        free(buffer);
        return NULL;
    }

    long word_index = 0;
    bool inside_word = false;

    __m256i spaces = _mm256_set1_epi8(' ');
    size_t i = 0;

    for (; i + 32 <= read_size; i += 32)
    {
        char* ptr = buffer + i;
        uint32_t sep_mask = 0;

        asm volatile (
            ".intel_syntax noprefix                 \n\t"

            "vmovdqu      ymm0, YMMWORD PTR [%[p]]  \n\t"
            "vpcmpeqb     ymm1, ymm0, %[sp]         \n\t"
            "vpmovmskb    %[mask], ymm1             \n\t"

            "vpandn       ymm2, ymm1, ymm0          \n\t"   // repace  ' ' to \0
            "vmovdqu      YMMWORD PTR [%[p]], ymm2  \n\t"

            ".att_syntax prefix                     \n\t"
            : [mask] "=r"(sep_mask)
            : [p]  "r"(ptr),
              [sp] "x"(spaces)
            : "ymm0", "ymm1", "ymm2", "ymm5", "memory"
        );

        uint32_t first_bit  = inside_word ? 0 : 1;
        uint32_t start_mask = (~sep_mask) & ((sep_mask << 1) | first_bit);

        while (start_mask)
        {
            unsigned bit_index = (unsigned)__builtin_ctz(start_mask);
            words[word_index++] = ptr + bit_index;
            start_mask &= start_mask - 1;
        }

        inside_word = (((sep_mask >> 31) & 1) == 0);
    }

    asm volatile ("vzeroupper" ::: "memory");

    for (; i < read_size; i++)
    {
        if (buffer[i] == ' ')
        {
            buffer[i] = '\0';
            inside_word = false;
        }
        else if (!inside_word)
        {
            words[word_index++] = &buffer[i];
            inside_word = true;
        }
    }

    *num_lines = word_index;

    if (*num_lines == 0)
    {
        free(words);
        free(buffer);
        return NULL;
    }

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