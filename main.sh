#!/bin/bash

perf stat -e cache-references,cache-misses ./main $1 
#perf stat -e cache-references,cache-misses --field-separator=, -o perf.csv  ./main # If you want to have perf data as .csv

exit 0

