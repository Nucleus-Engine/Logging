#include "Logger.h"
#include <sstream>
#include <iomanip>

Logger::Logger() : running_(true), level_(DEFAULT_LOG_LEVEL) {
    workerThread_ = std::thread(&Logger::worker, this);
}

Logger::~Logger() {
    running_ = false;
    cv_.notify_all();
    if (workerThread_.joinable()) {
        workerThread_.join();
    }
}

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

void Logger::setLogLevel(LogLevel level) {
    level_ = level;
}

void Logger::addSink(std::shared_ptr<LogSink> sink) {
    std::lock_guard<std::mutex> lock(queueMutex_);
    sinks_.push_back(std::move(sink));
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level < level_) return;
    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        queue_.emplace(level, message);
    }
    cv_.notify_one();
}

void Logger::worker() {
    while (running_) {
        std::unique_lock<std::mutex> lock(queueMutex_);
        cv_.wait(lock, [&]() { return !queue_.empty() || !running_; });

        while (!queue_.empty()) {
            LogMessage msg = queue_.front();
            queue_.pop();
            lock.unlock();

            if (msg.level >= level_)
            {
                // Format the message
                auto time = std::chrono::system_clock::to_time_t(msg.timestamp);
                std::stringstream ss;
                ss << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") << "]";
                ss << "[Thread " << msg.threadId << "]";

                switch (msg.level) {
                    case LogLevel::DEBUG: ss << "[DEBUG] "; break;
                    case LogLevel::INFO:  ss << "[INFO ] "; break;
                    case LogLevel::WARN:  ss << "[WARN ] "; break;
                    case LogLevel::ERROR: ss << "[ERROR] "; break;
                    case LogLevel::FATAL: ss << "[FATAL] "; break;
                }

                ss << msg.text;
                std::string formatted = ss.str();

                for (auto& sink : sinks_) {
                    sink->write(formatted);
                }
            }

            lock.lock();
        }
    }
}
