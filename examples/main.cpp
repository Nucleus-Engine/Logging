#include "ErrorLogger.h"

using namespace Nucleus;

void task(const std::string& message)
{
    WARN(message);
}

int main() {
    TRACE("Program started.");

    const std::string messageTemplate = "Logging message ";
    for (uint32_t idx = 0U; idx < 1'000U; ++idx)
    {
        char message[30] = "";
        sprintf(message, "%s%u", messageTemplate.c_str(), idx);
        task(message);
    }

    return 0;
}
