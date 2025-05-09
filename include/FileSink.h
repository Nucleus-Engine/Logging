#ifndef NUCLEUS_FILE_SINK_H
#define NUCLEUS_FILE_SINK_H

#include "LogSink.h"
#include <fstream>
#include <mutex>

class FileSink : public LogSink {
public:
    explicit FileSink(const std::string& filename);
    void write(const std::string& message) override;

private:
    std::ofstream file_;
    std::mutex mutex_;
};

#endif // #ifndef NUCLEUS_FILE_SINK_H
