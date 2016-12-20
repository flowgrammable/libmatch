#!/bin/bash
echo "=====1911====="
./optimized_libmatch/test/group_search_reviewed.test ~/libmatch/test/Benchmarks/newRule_less12_1911.txt ~/libmatch/test/Benchmarks/two_fields_64bit.txt ~/libmatch/test/Benchmarks/action1911.txt 18 >> nolimit.txt
./optimized_libmatch/test/group_noexpand_base.test ~/libmatch/test/Benchmarks/newRule_less12_1911.txt ~/libmatch/test/Benchmarks/two_fields_64bit.txt ~/libmatch/test/Benchmarks/action1911.txt >> nolimit.txt
./optimized_libmatch/test/group_advanced.test ~/libmatch/test/Benchmarks/newRule_less12_1911.txt ~/libmatch/test/Benchmarks/two_fields_64bit.txt ~/libmatch/test/Benchmarks/action1911.txt 2000000 >> nolimit.txt
./optimized_libmatch/test/group_nolimit.test ~/libmatch/test/Benchmarks/newRule_less12_1911.txt ~/libmatch/test/Benchmarks/two_fields_64bit.txt ~/libmatch/test/Benchmarks/action1911.txt >> nolimit.txt


echo "=====2925====="
./optimized_libmatch/test/group_search_reviewed.test ~/libmatch/test/Benchmarks/ping_big_64bit_original.txt ~/libmatch/test/Benchmarks/two_fields_64bit.txt ~/libmatch/test/Benchmarks/action2925.txt 18 >> nolimit.txt
./optimized_libmatch/test/group_noexpand_base.test ~/libmatch/test/Benchmarks/ping_big_64bit_original.txt ~/libmatch/test/Benchmarks/two_fields_64bit.txt ~/libmatch/test/Benchmarks/action2925.txt >> nolimit.txt
./optimized_libmatch/test/group_advanced.test ~/libmatch/test/Benchmarks/ping_big_64bit_original.txt ~/libmatch/test/Benchmarks/two_fields_64bit.txt ~/libmatch/test/Benchmarks/action2925.txt 2000000 >> result_try1.txt
./optimized_libmatch/test/group_nolimit.test ~/libmatch/test/Benchmarks/ping_big_64bit_original.txt ~/libmatch/test/Benchmarks/two_fields_64bit.txt ~/libmatch/test/Benchmarks/action2925.txt >> nolimit.txt

./optimized_libmatch/test/group_nolimit.test ~/libmatch/test/Benchmarks/461.txt ~/libmatch/test/Benchmarks/two_fields_64bit.txt ~/libmatch/test/Benchmarks/action461.txt
