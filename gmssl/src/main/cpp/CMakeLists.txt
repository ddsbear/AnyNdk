
cmake_minimum_required(VERSION 3.4.1)

include_directories(include)

add_library(
        native-lib
        SHARED
        native-lib.cpp)

## CMake Error: fix shared library text segment is not shareable
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wall -v -Wl,--no-warn-shared-textrel")

add_library(crypto STATIC IMPORTED)
set_target_properties(crypto
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}/libcrypto.a)

add_library(ssl STATIC IMPORTED)
set_target_properties(ssl
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}/libssl.a)


target_link_libraries( # Specifies the target library.
        native-lib
        crypto
        ssl
        ${log-lib})