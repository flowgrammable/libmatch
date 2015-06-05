# Copyright (c) 2015 Flowgrammable.org
# All rights reserved

find_package(Boost 1.58.0)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_library(match STATIC hash.cpp)
else()
  message(FATAL_ERROR "Must have Boost installed")
endif()  



