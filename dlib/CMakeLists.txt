cmake_minimum_required(VERSION 3.4.1)

set(BUILD_PATH ${CMAKE_SOURCE_DIR}/src/main/jniLibs)
set(DLIB_PATH ${CMAKE_SOURCE_DIR}/src/main/cpp/dlib)
set(DLIB_ANDROID_PATH ${CMAKE_SOURCE_DIR}/src/main/cpp/dlib_android)

set(source_files
    ${DLIB_ANDROID_PATH}/Java_com_lmy_dlib_Dlib.cpp
    ${DLIB_ANDROID_PATH}/dlib.cpp )

include(${DLIB_PATH}/cmake)

add_library( # Sets the name of the library.
             dlib-android

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ${source_files})

set_target_properties(dlib-android PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY
                      ${BUILD_PATH}/${ANDROID_ABI})

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

target_link_libraries(dlib-android
                      dlib::dlib
                      ${log-lib} )

SET(CMAKE_BUILE_TYPE "RELEASE")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
SET(CMAKE_BUILE_TYPE "DEBUG")
SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")