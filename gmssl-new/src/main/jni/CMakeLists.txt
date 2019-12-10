
cmake_minimum_required(VERSION 3.4.1)

include_directories(include)

add_library(
        native-gmssl
        SHARED
        native-gmssl.cpp
)

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
        native-gmssl
        crypto
        ssl
        ${log-lib})