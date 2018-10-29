cmake_minimum_required(VERSION 3.4.1)

set(distribution_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs)
set(pathToDlib ${CMAKE_SOURCE_DIR}/src/main/cpp)

include(${pathToDlib}/dlib/cmake)

add_library( # Sets the name of the library.
             dlib-android

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/dlib.cpp )

set_target_properties(dlib-android PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY
                      ${distribution_DIR}/${ANDROID_ABI})

target_link_libraries(dlib-android
                      dlib::dlib
                      log)
SET(CMAKE_BUILE_TYPE "RELEASE")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
SET(CMAKE_BUILE_TYPE "DEBUG")
SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")