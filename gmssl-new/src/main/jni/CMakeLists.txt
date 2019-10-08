cmake_minimum_required(VERSION 3.4.1)


include_directories(include)

add_library(
        mobishield
        SHARED
        mobishield.cpp
        cipher.cpp
)

## CMake Error: fix shared library text segment is not shareable
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wall -v -Wl,--no-warn-shared-textrel")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -ffunction-sections -fdata-sections -fvisibility=hidden")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections")

add_library(crypto STATIC IMPORTED)
set_target_properties(crypto
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}/libcrypto.a)

add_library(ssl STATIC IMPORTED)
set_target_properties(ssl
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}/libssl.a)


find_library(
        log-lib
        log)
target_link_libraries(
        mobishield
        crypto
        ssl
        ${log-lib})