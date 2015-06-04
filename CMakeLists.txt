# Copyright (c) 2015 Flowgrammable.org
# All rights reserved

cmake_minimum_required(VERSION 3.0)
project(match CXX)


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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(libmatch)
