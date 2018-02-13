cmake_minimum_required(VERSION 2.8)

project(logger)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

set(SOURCES
    core.cpp
    log.h
    clog.h
    logger.h
    logger_sink.h
    console_logger.h
    console_logger.cpp
    file_logger.h
    file_logger.cpp
    syslog_logger.h
    syslog_logger.cpp
    date.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION 1.0.0)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

add_executable(log-test log-test.c clog.h)
target_link_libraries(log-test ${PROJECT_NAME})

add_executable("log-test++" log-test.cpp log.h)
target_link_libraries("log-test++" ${PROJECT_NAME})
