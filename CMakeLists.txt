cmake_minimum_required(VERSION 3.15)
project(NucleusLogging)

set(CMAKE_CXX_STANDARD 17)

# Configurable log level
set(LOG_LEVEL "INFO" CACHE STRING "Default log level: DEBUG, INFO, WARN, ERROR, or FATAL")

# Map string to enum
if(LOG_LEVEL STREQUAL "DEBUG")
    set(LOG_LEVEL_ENUM "LogLevel::DEBUG")
elseif(LOG_LEVEL STREQUAL "INFO")
    set(LOG_LEVEL_ENUM "LogLevel::INFO")
elseif(LOG_LEVEL STREQUAL "WARN")
    set(LOG_LEVEL_ENUM "LogLevel::WARN")
elseif(LOG_LEVEL STREQUAL "ERROR")
    set(LOG_LEVEL_ENUM "LogLevel::ERROR")
elseif(LOG_LEVEL STREQUAL "FATAL")
    set(LOG_LEVEL_ENUM "LogLevel::FATAL")
else()
    message(FATAL_ERROR "Invalid LOG_LEVEL: ${LOG_LEVEL}")
endif()

# Add logger library
add_library(NucleusLogging
    src/Logger.cpp
    src/ConsoleSink.cpp
    src/RotatingFileSink.cpp
)

target_include_directories(NucleusLogging PUBLIC include)

# Pass the log level macro to the compiler
target_compile_definitions(NucleusLogging PUBLIC DEFAULT_LOG_LEVEL=${LOG_LEVEL_ENUM})

add_executable(example examples/main.cpp)
target_link_libraries(example NucleusLogging)
