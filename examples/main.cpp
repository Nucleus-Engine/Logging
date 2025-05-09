#include "Logger.h"
#include "ConsoleSink.h"
#include "RotatingFileSink.h"
#include "threads.h"
#include "string.h"
#include "math.h"

int randi(int lo, int hi)
{
    int n = hi - lo + 1;
    int i = rand() % n;
    if (i < 0) i = -i;
    return lo + i;
}

void task(const std::string& message, int delay)
{
    auto& logger = Logger::instance();

    std::this_thread::sleep_for(std::chrono::milliseconds(delay));

    logger.log(LogLevel::ERROR, message);
}

int main() {
    auto& logger = Logger::instance();

    logger.addSink(std::make_shared<ConsoleSink>());
    logger.addSink(std::make_shared<RotatingFileSink>("app.log"));

    logger.log(LogLevel::INFO, "Program started with rotating file logger.");

    std::vector<std::thread> _threads;
    const std::string messageTemplate = "Logging On Thread: ";
    for (size_t idx = 0U; idx < 1'000U; ++idx)
    {
        char message[30] = "";
        sprintf(message, "%s%lu", messageTemplate.c_str(), idx);
        _threads.emplace_back(std::thread(task, message, randi(0, 100)));
    }

    for (auto& th : _threads)
    {
        th.join();
    }
}
