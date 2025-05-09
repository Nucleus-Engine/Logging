#include "FileSink.h"

FileSink::FileSink(const std::string& filename) {
    file_.open(filename, std::ios::app);
}

void FileSink::write(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    file_ << message << std::endl;
}
