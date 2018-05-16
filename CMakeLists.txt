cmake_minimum_required(VERSION 2.8)

project(chimp_jni)
set(CMAKE_CXX_FLAGS "-std=c++11")
add_definitions(-Wall -Wno-unknown-pragmas)


find_package (Java REQUIRED)
find_package (JNI REQUIRED)
include (UseJava)

if (JNI_FOUND)
    message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif()

include_directories (${JNI_INCLUDE_DIRS})

include_directories(include)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
