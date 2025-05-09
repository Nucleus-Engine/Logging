#ifndef NUCLEUS_ROTATING_FILE_SINK_H
#define NUCLEUS_ROTATING_FILE_SINK_H

#include "LogSink.h"
#include <fstream>
#include <mutex>
#include <string>

class RotatingFileSink : public LogSink {
public:
    explicit RotatingFileSink(const std::string& baseFilename);
    void write(const std::string& message) override;

private:
    void rotateIfNeeded();
    std::string getTimestampedFilename() const;

    std::ofstream file_;
    std::string baseFilename_;
    std::mutex mutex_;
};

#endif // #ifndef NUCLEUS_ROTATING_FILE_SINK_H
