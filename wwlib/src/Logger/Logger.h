#pragma once

#ifndef LOGGER_H
#define LOGGER_H

#include <pch.h>
#include <Helper/Utils.h>

class Logger {
public:
    enum class LogLevel {
        Info,
        Warn,
        Error,
        Debug,
    };

    template<typename... Args>
    static void log(const std::string& format, LogLevel level, bool toConsole = true, bool color = true, Args... args);

    template<typename... Args>
    static void info(const std::string& format, Args... args);

    template<typename... Args>
    static void warning(const std::string& format, Args... args);

    template<typename... Args>
    static void error(const std::string& format, Args... args);

    template<typename... Args>
    static void debug(const std::string& format, Args... args);

private:
    static std::ofstream log_file;
    static std::string lastMessage;
    static int lastMessageCount;
    static bool isInitialized;


    static void initializeLogger();
    static std::string generateLogFileName(const std::string& directory);
    static std::string getCurrentTime();
    static std::string getLevelString(LogLevel level);
    static void setConsoleColor(LogLevel level);
    static void resetConsoleColor();
    static void flushLastMessage();
    static void createDirectoryIfNotExists(const std::string& directory);
    static std::string formatString(const char* format, ...);


    static std::string logDirectory;
};


template<typename... Args>
void Logger::log(const std::string& format, LogLevel level, bool toConsole, bool color, Args... args) {
    if (!isInitialized) {
        initializeLogger();
    }

    std::string levelStr = getLevelString(level);
    std::string timeStamp = getCurrentTime();
    std::string logMessage = "[" + timeStamp + "] [" + levelStr + "] " + formatString(format.c_str(), args...);

    if (logMessage == lastMessage) {
        lastMessageCount++;
    }
    else {
        if (lastMessageCount > 0) {
            flushLastMessage();
        }
        lastMessage = logMessage;
        lastMessageCount = 1;
    }

    if (toConsole) {
        if (color) {
            setConsoleColor(level);
        }
        std::cout << logMessage << std::endl;
        if (color) {
            resetConsoleColor();
        }
    }
}


template<typename... Args>
void Logger::info(const std::string& format, Args... args) {
    log(format, LogLevel::Info, true, true, args...);
}

template<typename... Args>
void Logger::warning(const std::string& format, Args... args) {
    log(format, LogLevel::Warn, true, true, args...);
}

template<typename... Args>
void Logger::error(const std::string& format, Args... args) {
    log(format, LogLevel::Error, true, true, args...);
}

template<typename... Args>
void Logger::debug(const std::string& format, Args... args) {
    log(format, LogLevel::Debug, true, true, args...);
}


#define LOG_INFO(format, ...)       Logger::info(format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...)    Logger::warning(format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...)      Logger::error(format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...)      Logger::debug(format, ##__VA_ARGS__)

#endif // LOGGER_H