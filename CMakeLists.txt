cmake_minimum_required(VERSION 3.16)
project(ebmlio)

set(CMAKE_CXX_STANDARD 17)

add_library(ebmlio SHARED library.cpp library.h)