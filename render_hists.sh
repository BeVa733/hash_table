#!/bin/bash

mkdir -p histograms

python py_scripts/plot_hist.py results/tables/one_hash.csv         results/histograms/one_hash.png
python py_scripts/plot_hist.py results/tables/first_ascii_hash.csv results/histograms/first_ascii_hash.png
python py_scripts/plot_hist.py results/tables/lenth_hash.csv       results/histograms/lenth_hash.png
python py_scripts/plot_hist.py results/tables/ascii_sum_hash.csv   results/histograms/ascii_sum_hash.png
python py_scripts/plot_hist.py results/tables/hash_rol_xor.csv     results/histograms/hash_rol_xor.png
python py_scripts/plot_hist.py results/tables/gnu_hash.csv         results/histograms/gnu_hash.png
python py_scripts/plot_hist.py results/tables/crc32c_hash.csv      results/histograms/crc32c_hash.png