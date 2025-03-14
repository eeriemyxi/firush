cmake_minimum_required(VERSION 3.14)
project(firush)

include(FetchContent)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed. Please create a separate build directory!")
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(CURL REQUIRED)
if(NOT CURL_FOUND)
    message(FATAL_ERROR "CURL not found. Please install CURL.")
endif()

add_library(libfirush SHARED src/firush.c)
set_target_properties(libfirush PROPERTIES OUTPUT_NAME "firush")

add_executable(firush src/firush.c src/main.c)

target_link_libraries(firush PRIVATE CURL::libcurl)
target_link_libraries(libfirush PRIVATE CURL::libcurl)
