#!/bin/bash
./build/test/group_search_reviewed.test ~/libmatch/test/Benchmarks/207.txt ~/libmatch/test/Benchmarks/two_fields_64bit.txt >> result_try.txt
./build/test/linear_arbitrary.test ~/libmatch/test/Benchmarks/207.txt ~/libmatch/test/Benchmarks/two_fields_64bit.txt >> result_try.txt


