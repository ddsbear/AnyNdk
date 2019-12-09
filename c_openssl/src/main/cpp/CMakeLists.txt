
cmake_minimum_required(VERSION 3.4.1)

include_directories(include)

add_library(
        native-cipher
        SHARED
        native-lib.cpp
        cipher.cpp
        utils.cpp)

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

target_link_libraries( # Specifies the target library.
        native-cipher
        crypto
        ssl
        ${log-lib})