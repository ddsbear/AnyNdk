
cmake_minimum_required(VERSION 3.4.1)

include_directories(include)

add_library(
        parson-test
        SHARED
        parson-test.cpp)


add_library(parson STATIC parson.c)


find_library(
        log-lib
        log)

target_link_libraries( # Specifies the target library.
        parson-test
        parson
        ${log-lib})