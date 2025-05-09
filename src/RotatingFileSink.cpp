#include "RotatingFileSink.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

RotatingFileSink::RotatingFileSink(const std::string& baseFilename)
    : baseFilename_(baseFilename) {
    rotateIfNeeded();
    file_.open(baseFilename_, std::ios::trunc); // fresh file
}

void RotatingFileSink::rotateIfNeeded() {
    if (!fs::exists(baseFilename_)) return;

    std::string newName = getTimestampedFilename();
    try {
        fs::rename(baseFilename_, newName);
    } catch (const std::exception& e) {
        std::cerr << "Log rotation failed: " << e.what() << std::endl;
    }
}

std::string RotatingFileSink::getTimestampedFilename() const {
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);

    std::ostringstream oss;
    oss << baseFilename_ << "-";
    oss << std::put_time(std::localtime(&timeT), "%Y%m%d-%H%M%S") << ".log";
    return oss.str();
}

void RotatingFileSink::write(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (file_.is_open()) {
        file_ << message << std::endl;
    }
}
