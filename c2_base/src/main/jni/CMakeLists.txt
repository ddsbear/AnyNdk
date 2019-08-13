
cmake_minimum_required(VERSION 3.4.1)

aux_source_directory(. SRC_SOURCE)

include_directories(include)

add_library(
        native-lib
        STATIC
        ${SRC_SOURCE})


target_link_libraries(
        native-lib
        log)