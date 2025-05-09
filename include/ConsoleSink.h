#ifndef NUCLEUS_CONSOLE_SINK_H
#define NUCLEUS_CONSOLE_SINK_H

#include "LogSink.h"
#include <iostream>
#include <mutex>

class ConsoleSink : public LogSink {
public:
    void write(const std::string& message) override;

private:
    std::mutex mutex_;
};

#endif // #ifndef NUCLEUS_CONSOLE_SINK_H
