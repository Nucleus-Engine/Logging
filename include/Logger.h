#ifndef NUCLEUS_LOGGING_H
#define NUCLEUS_LOGGING_H

#include "LogSink.h"
#include "LogMessage.h"
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <memory>
#include <atomic>

class Logger {
public:
    static Logger& instance();
    void log(LogLevel level, const std::string& message);
    void addSink(std::shared_ptr<LogSink> sink);
    void setLogLevel(LogLevel level);
    ~Logger();

private:
    Logger();
    void worker();

    std::vector<std::shared_ptr<LogSink>> sinks_;
    std::queue<LogMessage> queue_;
    std::mutex queueMutex_;
    std::condition_variable cv_;
    std::atomic<bool> running_;
    std::thread workerThread_;
    LogLevel level_;
};

#endif // #ifndef NUCLEUS_LOGGING_H
