#!/bin/bash

touch coverage_results;

for i in *.gcno; do
	gcov -r "${i/gcno/cpp}" >> coverage_results;
done