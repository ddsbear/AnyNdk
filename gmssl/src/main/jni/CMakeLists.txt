cmake_minimum_required(VERSION 3.4.1)

include_directories(include)


add_library(gmcipher
        SHARED
        GmCipher.cpp
        log.cpp)


add_library(myssl STATIC IMPORTED)
set_target_properties(myssl PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_ANDROID_ARCH_ABI}/libmyssl.a)

add_library(mycrypto STATIC IMPORTED)
set_target_properties(myssl PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_ANDROID_ARCH_ABI}/libmycrypto.a)


link_libraries(
        gmcipher
        myssl
        mycrypto
        log
)