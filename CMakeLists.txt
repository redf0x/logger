cmake_minimum_required(VERSION 2.8)

project(logger)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -I${CMAKE_SOURCE_DIR}/include")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${CMAKE_SOURCE_DIR}/include")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

set(SOURCES
    src/core.cpp
    include/log.h
    include/clog.h
    include/logger.h
    include/logger_sink.h
    include/console_logger.h
    src/console_logger.cpp
    include/file_logger.h
    src/file_logger.cpp
    include/syslog_logger.h
    src/syslog_logger.cpp
    include/date.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION 1.0.0)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

add_executable(log-test log-test.c include/clog.h)
target_link_libraries(log-test ${PROJECT_NAME})

add_executable("log-test++" log-test.cpp include/log.h)
target_link_libraries("log-test++" ${PROJECT_NAME})
