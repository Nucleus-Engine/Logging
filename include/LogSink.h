#ifndef NUCLEUS_LOG_SINK_H
#define NUCLEUS_LOG_SINK_H

#include "Common.h"
#include <string>

class LogSink {
public:
    DllExport virtual void write(const std::string& message) = 0;
    DllExport virtual ~LogSink() = default;
};

#endif // #ifndef NUCLEUS_LOG_SINK_H
