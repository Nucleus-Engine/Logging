cmake_minimum_required(VERSION 3.15)
project(NucleusLogging CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LOG_LEVEL "WARN" CACHE STRING "Log level: TRACE, DEBUG, WARN, ERROR, or FATAL")
set_property(CACHE LOG_LEVEL PROPERTY STRINGS TRACE DEBUG WARN ERROR FATAL)

option(CONSOLE "Force console output (mirrors meson's 'console' option)" OFF)

if(LOG_LEVEL STREQUAL "TRACE")
    set(LOG_LEVEL_DEFINE 1)
elseif(LOG_LEVEL STREQUAL "DEBUG")
    set(LOG_LEVEL_DEFINE 2)
elseif(LOG_LEVEL STREQUAL "WARN")
    set(LOG_LEVEL_DEFINE 3)
elseif(LOG_LEVEL STREQUAL "ERROR")
    set(LOG_LEVEL_DEFINE 4)
elseif(LOG_LEVEL STREQUAL "FATAL")
    set(LOG_LEVEL_DEFINE 5)
else()
    message(FATAL_ERROR "Invalid LOG_LEVEL: ${LOG_LEVEL}")
endif()

add_library(NucleusLogging INTERFACE)

target_include_directories(NucleusLogging INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(NucleusLogging INTERFACE
    LOG_LEVEL=${LOG_LEVEL_DEFINE}
)

if(CONSOLE)
    target_compile_definitions(NucleusLogging INTERFACE CONSOLE=true)
endif()

install(FILES include/ErrorLogger.h
        DESTINATION include/NucleusLogging)

add_subdirectory(examples)