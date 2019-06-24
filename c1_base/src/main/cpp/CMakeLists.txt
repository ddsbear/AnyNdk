
cmake_minimum_required(VERSION 3.4.1)

aux_source_directory(. SRC_SOURCE)
aux_source_directory(native5 SRC_SOURCE2)

include_directories(native5)

add_library(
        native-lib
        SHARED
        ${SRC_SOURCE}
        ${SRC_SOURCE2})


target_link_libraries(
        native-lib
        log)