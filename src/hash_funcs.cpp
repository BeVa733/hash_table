#include "hash_table.h"

uint32_t one_hash(const char* word)
{
    return 1;
}

uint32_t first_ascii_hash(const char* word)
{
    return word[0];
}

uint32_t lenth_hash(const char* word)
{
    return strlen(word);
}

uint32_t ascii_sum_hash(const char* word)
{
    uint32_t sum = 0;
    int index = 0;

    while(word[index] != '\0')
    {
        sum += word[index];
        index++;
    }

    return sum;
}

uint32_t hash_rol_xor(const char* word)
{
    if (!word || word[0] == 0) 
        return 0;

    uint32_t hash = word[0];
    for (size_t i = 1; word[i] != 0; i++) 
    {
        hash = ((hash << 1) | (hash >> 61));
        hash ^= word[i];
    }

    return hash;
}

uint32_t gnu_hash(const char* word)
{
    uint32_t hash = 5381;

    for (; *word; word++)
        hash = (hash << 5) + hash + *word;

    return hash;
}

uint32_t crc32c_hash(const char* word)
{
    if (word == NULL)
        return 0;

    uint32_t crc = 0xFFFFFFFF;

    while (*word) 
    {
        crc ^= *word;

        for (int j = 0; j < 8; ++j) 
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0x82F63B78;
            else
                crc >>= 1;
        }

        word++;
    }

    return crc ^ 0xFFFFFFFF;
}