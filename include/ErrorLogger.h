#pragma once

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <mutex>

inline std::tm get_local_time(const std::time_t& timer) {
    std::tm bt{};
#if defined(_MSC_VER) || defined(__MINGW32__)
    // Windows implementation (arguments reversed from POSIX)
    localtime_s(&bt, &timer);
#else
    // POSIX implementation (Linux, macOS)
    localtime_r(&timer, &bt);
#endif
    return bt;
}

/**
 * LOG_LEVELS
 * TRACE >= 1
 * DEBUG >= 2
 * WARN >= 3
 * ERROR >= 4
 * FATAL *always*
 */

namespace Nucleus
{
/**
 * @brief Singleton class for logging errors to file (or if file handling fails, console)
 *
 * @warning This class will go from being a singleton to a static namespace in the future
 */
class ErrorLogger
{
  public:
    /**
     * @brief Gets the instance of the ErrorLogger singleton
     *
     * @return ErrorLogger& a reference to the singleton
     */
    static ErrorLogger& getInstance()
    {
        static ErrorLogger _instance(s_pendingFilename);

        return _instance;
    }

    /**
     * @brief Sets the file name to use. Defaults to "output.log". This function is
     *        nonfunctional after the first use of the ErrorLogger singleton.
     *
     * @param filename the filename to use
     */
    static void setFilename(const std::string& filename)
    {
        s_pendingFilename = filename;
    }

    static void setConsoleOnly() { getInstance().m_canWriteToFile = false; }

    /**
     * @brief logs the given string with a timestamp that looks like "Mon Nov 20
     * 23:28:22 2023"
     *
     * @param logString the string to log
     */
    void log(const std::string& logString)
    {
        const auto currentTime = std::chrono::system_clock::now();
        const std::time_t now = std::chrono::system_clock::to_time_t(currentTime);

        char timeBuffer[80];
        {
            std::tm tmBuf = get_local_time(now);
            std::strftime(timeBuffer, sizeof(timeBuffer), "%c", &tmBuf);
        }

        std::string output(timeBuffer);
        output.append("\t" + logString + "\n");

        std::lock_guard<std::mutex> lock(m_mutex);

        if (!m_canWriteToFile)
        {
            std::cout << output;
            return;
        }

        m_file.open(m_filename, std::ios::app);
        if (!m_file)
        {
            std::cout << output;
            m_canWriteToFile = false;
            return;
        }
        m_file << output;
        m_file.close();
    }

    ErrorLogger(const ErrorLogger& other) = delete;

    void operator=(const ErrorLogger& rhs) = delete;

  private:
    explicit ErrorLogger(std::string filename = "output.log")
        : m_filename(std::move(filename)), m_mutex()
    {
#ifndef CONSOLE
        m_canWriteToFile = true;
        m_file.open(m_filename, std::ios::out);
        if (!m_file)
        {
            std::cout << "Failed to open " << m_filename << "\n";
            m_canWriteToFile = false;
        }

        if (m_file.is_open())
        {
            m_file.close();
        }
#else /* CONSOLE defined*/
        m_canWriteToFile = false;
#endif /* #ifndef CONSOLE */
    }

    ~ErrorLogger()
    {
        if (m_file.is_open())
        {
            m_file.close();
        }
    }

    std::string m_filename; //!< logfile name
    std::fstream m_file;    //!< internal file object
    bool m_canWriteToFile;  //!< true if there has been no issue opening the file, false if there has been
    std::mutex m_mutex;     //!< mutex for thread safety

    static inline std::string s_pendingFilename = "output.log";
};

} // namespace Nucleus

#define COMMON_ENDING(message)                                                                     \
    std::string("\t\t") + std::string(message) + ":\t\t" + __FILE__ + "[" +                        \
        std::to_string(__LINE__) + "]"

#if LOG_LEVEL <= 1
/**
 * @brief Logs at log level 1 or below. Use to trace a path through code
 * execution
 *
 * @param message must be able to convert to std::string
 */
#define TRACE(message)                                                                             \
    do {                                                                                           \
        std::string output = "[TRACE]" + COMMON_ENDING(message);                                   \
        Nucleus::ErrorLogger::getInstance().log(output);                                           \
    } while (false)
#else
#define TRACE(message) ;
#endif // LOG_LEVEL <= 1

#if LOG_LEVEL <= 2
/**
 * @brief Logs at log level 2 or below. Use for debugging
 *
 * @param message must be able to convert to std::string
 */
#define DEBUG(message)                                                                             \
    do {                                                                                           \
        std::string output = "[DEBUG]" + COMMON_ENDING(message);                                   \
        Nucleus::ErrorLogger::getInstance().log(output);                                           \
    } while (false)
#else
#define DEBUG(message) ;
#endif // LOG_LEVEL <= 2

#if LOG_LEVEL <= 3
/**
 * @brief Logs at log level 3 or below. Use when code execution can continue,
 * but will likely be impacted
 *
 * @param message must be able to convert to std::string
 */
#define WARN(message)                                                                              \
    do {                                                                                           \
        std::string output = "[WARN]" + COMMON_ENDING(message);                                    \
        Nucleus::ErrorLogger::getInstance().log(output);                                           \
    } while (false)
#else
#define WARN(message) ;
#endif // LOG_LEVEL <= 3

#if LOG_LEVEL <= 4
/**
 * @brief Logs at log level 4 or below. Use when code execution can likely
 * continue, but may be severely affected
 *
 * @param message must be able to convert to std::string
 */
#define ERROR(message)                                                                             \
    do {                                                                                           \
        std::string output = "[ERROR]" + COMMON_ENDING(message);                                   \
        Nucleus::ErrorLogger::getInstance().log(output);                                           \
    } while (false)
#else
#define ERROR(message) ;
#endif // LOG_LEVEL <= 4

/**
 * @brief Logs at any log level. Use when code execution cannot continue
 *
 * @param message must be able to convert to std::string
 */
#define FATAL(message)                                                                             \
    do {                                                                                           \
        std::string output = "[FATAL]" + COMMON_ENDING(message);                                   \
        Nucleus::ErrorLogger::getInstance().log(output);                                           \
    } while (false)
