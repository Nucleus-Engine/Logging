#ifndef NUCLEUS_LOG_SINK_H
#define NUCLEUS_LOG_SINK_H

#include <string>

class LogSink {
public:
    virtual void write(const std::string& message) = 0;
    virtual ~LogSink() = default;
};

#endif // #ifndef NUCLEUS_LOG_SINK_H
