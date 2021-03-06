# Copyright (c) 2015 Flowgrammable.org
# All rights reserved

#cmake_minimum_required(VERSION 3.0)
project(match CXX)
enable_testing()

set(env{CTEST_OUTPUT_ON_FAILURE} TRUE)

# Configure the compiler.
#
# We effectively require a functioning C++11 implementation.
# For Clang, use the libc++ as the standard library.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.8")
    message(FATAL_ERROR "Requires GCC version 4.8 or greater")
  endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")  
endif()

# Compile in C++11.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


# Require Boost C++ Libraries.
find_package(Boost 1.54.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(wildcard.test wildcard.cpp)
add_executable(hash.test hash.cpp)
add_executable(action_generator.test action_generator.cpp)
#add_executable(basic_trie.test basic_trie.cpp)
add_executable(basic_trie_group.test basic_trie_group.cpp)
#add_executable(basic_trie_group_new.test basic_trie_group_new.cpp)
#add_executable(trie_wo_priority.test trie_wo_priority.cpp)
#add_executable(trie_with_merge.test trie_with_merge.cpp)
#add_executable(trie_with_merge1.test trie_with_merge1.cpp)
#add_executable(trie_with_merge1_sort.test trie_with_merge1_sort.cpp)
add_executable(random_vector.test random_vector.cpp)
#add_executable(trie_backup.test trie_backup.cpp)
add_executable(linear_arbitrary.test linear_arbitrary.cpp)
add_executable(group_search_reviewed.test group_search_reviewed.cpp)
add_executable(group_noexpand_base.test group_noexpand_base.cpp)
add_executable(bw_noexpand_sort.test bw_noexpand_sort.cpp)
add_executable(group_advanced.test group_advanced.cpp)
add_executable(group_nolimit.test group_nolimit.cpp)
add_executable(bitweaving.test bitweaving.cpp)
add_executable(random_10rules.test random_10rules.cpp)
add_executable(ping_noexpand.test ping_noexpand.cpp)
add_executable(group_noexpand_merge.test group_noexpand_merge.cpp)
add_executable(samememory_group_optimized.test samememory_group_optimized.cpp)
add_executable(optimized_group_search_reviewed.test optimized_group_search_reviewed.cpp)
#add_executable(generate_keys_num.test generate_keys_num.cpp)


#add_test(test_generate_keys_num.test)
add_test(test_wildcard wildcard.test)
add_test(test_hash hash.test)
add_test(test_ping_noexpand ping_noexpand.test)
add_test(test_action_generator action_generator.test)
#add_test(test_basic_trie basic_trie.test)
#add_test(test_trie_backup trie_backup.test)
add_test(test_basic_trie_group basic_trie_group.test)
#add_test(test_basic_trie_group_new basic_trie_group_new.test)
#add_test(test_trie_wo_priority trie_wo_priority.test)
#add_test(test_trie_with_merge trie_with_merge.test)
add_test(test_group_search_reviewed group_search_reviewed.test)
add_test(test_random_10rules random_10rules.test)
add_test(test_group_noexpand_base group_noexpand_base.test)
add_test(test_bw_noexpand_sort bw_noexpand_sort.test)
add_test(test_group_advanced group_advanced.test)
add_test(test_group_nolimit group_nolimit.test)
add_test(test_bitweaving bitweaving.test)
add_test(test_group_noexpand_merge group_noexpand_merge.test)
add_test(test_optimized_group_search_reviewed optimized_group_search_reviewed.test)
add_test(test_samememory_group_optimized samememory_group_optimized.test)
#add_test(test_trie_with_merge1 trie_with_merge1.test)
add_test(random_vector random_vector.test)
#add_test(test_trie_with_merge1_sort trie_with_merge1_sort.test)
add_test(test_linear_arbitrary linear_arbitrary.test)
