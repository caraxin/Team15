#!/bin/bash

touch coverage_results;

for i in *.gcno; do
	gcov -r "${i/gcno/cc}" >> coverage_results;
done

# for i in ./nginx-configparser/*.gcno; do
# 	gcov -r "${i/gcno/cc}" >> coverage_results;
# done
