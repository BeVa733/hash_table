#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"

struct hash_info_t
{
    const char* name;
    hash_function func;
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <normalized.txt> [table_size]\n", argv[0]);
        return 1;
    }

    int table_size = 4001;
    if (argc >= 3)
        table_size = atoi(argv[2]);

    hash_info_t hashes[] =
    {
        {"one_hash",         one_hash        },
        {"first_ascii_hash", first_ascii_hash},
        {"lenth_hash",       lenth_hash      },
        {"ascii_sum_hash",   ascii_sum_hash  },
        {"hash_rol_xor",     hash_rol_xor    },
        {"gnu_hash",         gnu_hash        },
        {"crc32c_hash",      crc32c_hash     },
    };

    const int n_hashes = (int)(sizeof(hashes) / sizeof(hashes[0]));

    for (int i = 0; i < n_hashes; i++)
    {
        printf("table creation\n");
        table_t table = {};
        if (table_ctor(&table, table_size, hashes[i].func) != 0)
        {
            printf("Ctor failed for %s\n", hashes[i].name);
            continue;
        }
        printf("load table\n");

        load_table(&table, argv[1]);
        
        printf("end loading\n");

        char csv_name[128] = "";
        snprintf(csv_name, sizeof(csv_name), "%s.csv", hashes[i].name);

        dump_table_hist(&table, csv_name);

        printf("%-16s elems = %d, load_factor = %.4lf\n",
               hashes[i].name,
               table_elem_count(&table),
               (double)table_elem_count(&table) / (double)table.size);

        table_dtor(&table);
    }

    return 0;
}