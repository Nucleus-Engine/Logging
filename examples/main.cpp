#include "ErrorLogger.h"
#include <thread>
#include <vector>
#include <string>

using namespace Nucleus;

int randi(int lo, int hi)
{
    int n = hi - lo + 1;
    int i = rand() % n;
    if (i < 0) i = -i;
    return lo + i;
}

void task(std::string message, int delay)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    ERROR(message);
}

int main() {
    TRACE("Program started.");

    std::vector<std::thread> _threads;

    const std::string messageTemplate = "Logging message ";
    for (uint32_t idx = 0U; idx < 1'000U; ++idx)
    {
        std::string message = messageTemplate + std::to_string(idx);
        _threads.emplace_back(task, std::move(message), randi(0, 100));
    }

    for (auto& th : _threads)
    {
        th.join();
    }

    return 0;
}