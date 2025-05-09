#ifndef NUCLEUS_LOG_LEVEL_H
#define NUCLEUS_LOG_LEVEL_H


enum class LogLevel {
    DEBUG = 0,
    INFO,
    WARN,
    ERROR,
    FATAL
};

#ifndef DEFAULT_LOG_LEVEL
#define DEFAULT_LOG_LEVEL LogLevel::INFO
#endif

#endif // #ifndef NUCLEUS_LOG_LEVEL_H
