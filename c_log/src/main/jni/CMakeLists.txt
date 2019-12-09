
cmake_minimum_required(VERSION 3.4.1)


include_directories(include)
include_directories(utils)

add_library(
        native-log
        SHARED
        native-log.cpp
        utils/util.cpp
        log.cpp)


target_link_libraries(
        native-log
        log)