project(ownfactory)

cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

option(BUILD_WIN "True if WIN False if x86_64" OFF)

if (BUILD_WIN)
	set(CMAKE_C_COMPILER   i686-w64-mingw32-gcc)
	set(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
endif()

set(SOURCES factory.cpp)

# for convenient IDE job
set(HEADERS factory.h)


add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ../api)

