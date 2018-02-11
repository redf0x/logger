cmake_minimum_required(VERSION 2.8)

project(logger)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

set(SOURCES
    main.cpp
    logger.cpp
    logger.h
    date.h
)

add_executable(${PROJECT_NAME} ${SOURCES})
