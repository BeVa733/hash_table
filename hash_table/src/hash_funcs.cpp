#include <string.h>

#include "hash_table.h"

uint32_t one_hash(const char* word)
{
    (void)word;
    return 1;
}

uint32_t first_ascii_hash(const char* word)
{
    if (word == NULL || word[0] == '\0')
        return 0;

    return (unsigned char)word[0];
}

uint32_t lenth_hash(const char* word)
{
    if (word == NULL)
        return 0;

    return (uint32_t)strlen(word);
}

uint32_t ascii_sum_hash(const char* word)
{
    if (word == NULL)
        return 0;

    uint32_t sum = 0;

    for (int i = 0; word[i] != '\0'; i++)
        sum += (unsigned char)word[i];

    return sum;
}

uint32_t hash_rol_xor(const char* word)
{
    if (!word || word[0] == 0) 
        return 0;

    uint32_t hash = word[0];

    for (int i = 1; word[i] != '\0'; i++) 
    {
        hash = (hash << 1) | (hash >> 31);
        hash ^= (unsigned char)word[i];
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
        crc ^= (unsigned char)(*word);

        for (int j = 0; j < 8; j++)
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