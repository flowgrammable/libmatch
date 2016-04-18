# Copyright (c) 2015 Flowgrammable.org
# All rights reserved

cmake_minimum_required(VERSION 3.0)
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
find_package(Boost 1.55.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(wildcard.test wildcard.cpp)
add_executable(hash.test hash.cpp)
add_executable(basic_trie.test basic_trie.cpp)
add_executable(linear_arbitrary.test linear_arbitrary.cpp)

add_test(test_wildcard wildcard.test)
add_test(test_hash hash.test)
add_test(test_basic_trie basic_trie.test)
add_test(test_linear_arbitrary linear_arbitrary.test)
