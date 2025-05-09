#ifndef NUCLEUS_LOG_MESSAGE_H
#define NUCLEUS_LOG_MESSAGE_H

#include "LogLevel.h"
#include <string>
#include <thread>
#include <chrono>

struct LogMessage {
    LogLevel level;
    std::string text;
    std::chrono::system_clock::time_point timestamp;
    std::thread::id threadId;

    LogMessage(LogLevel lvl, const std::string& msg)
        : level(lvl), text(msg), timestamp(std::chrono::system_clock::now()), threadId(std::this_thread::get_id()) {}
};

#endif // #ifndef NUCLEUS_LOG_MESSAGE_H
