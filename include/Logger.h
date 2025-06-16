#ifndef NUCLEUS_LOGGING_H
#define NUCLEUS_LOGGING_H

#include "Common.h"
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
    DllExport static Logger& instance();
    DllExport void log(LogLevel level, const std::string& message);
    DllExport void addSink(std::shared_ptr<LogSink> sink);
    DllExport void setLogLevel(LogLevel level);
    DllExport void shutdown();

private:
    Logger();
    ~Logger();
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
