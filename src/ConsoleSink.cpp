#include "ConsoleSink.h"

void ConsoleSink::write(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << message << std::endl;
}
