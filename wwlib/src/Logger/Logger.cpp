#include <pch.h>
#include "Logger.h"


std::ofstream Logger::log_file;
std::string Logger::lastMessage;
int Logger::lastMessageCount = 0;
bool Logger::isInitialized = false;

#ifdef _DEBUG
std::string Logger::logDirectory;
#else
std::string Logger::logDirectory = "logs";
#endif

void Logger::initializeLogger() {
    if (!isInitialized) {
        createDirectoryIfNotExists(logDirectory);
        std::string filename = generateLogFileName(logDirectory);
        log_file.open(filename, std::ios::app);
        if (!log_file.is_open()) {
            throw std::runtime_error("Cannot open log file");
        }
        isInitialized = true;
    }
}

std::string Logger::generateLogFileName(const std::string& directory) {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now = *std::localtime(&time_t_now);

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << directory << "/log_"
        << std::put_time(&tm_now, "%Y-%m-%d_%H-%M-%S")
        << '-' << std::setfill('0') << std::setw(3) << ms.count()
        << ".log";
    return oss.str();
}

std::string Logger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now = *std::localtime(&time_t_now);

    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Logger::getLevelString(LogLevel level) {
    switch (level) {
    case LogLevel::Info:
        return "INFO";
    case LogLevel::Warn:
        return "WARNING";
    case LogLevel::Error:
        return "ERROR";
    case LogLevel::Debug:
        return "DEBUG";
    default:
        return "UNKNOWN";
    }
}

void Logger::setConsoleColor(LogLevel level) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (level) {
    case LogLevel::Info:
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        break;
    case LogLevel::Warn:
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        break;
    case LogLevel::Error:
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        break;
    case LogLevel::Debug:
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        break;
    }
#else
    switch (level) {
    case LogLevel::Info:
        std::cout << "\033[32m"; // Green
        break;
    case LogLevel::Warn:
        std::cout << "\033[33m"; // Orange
        break;
    case LogLevel::Error:
        std::cout << "\033[31m"; // Red
        break;
    case LogLevel::Debug:
        std::cout << "\033[36m"; // Blue
        break;
    }
#endif
}

void Logger::resetConsoleColor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    std::cout << "\033[0m"; // Reset
#endif
}


void Logger::flushLastMessage() {
    std::string finalMessage = lastMessage + " (x" + std::to_string(lastMessageCount) + ")";
    log_file << finalMessage << std::endl;
    if (lastMessageCount > 1) {
        std::cout << finalMessage << std::endl;
    }
    lastMessageCount = 0;
}

void Logger::createDirectoryIfNotExists(const std::string& directory) {
    if (logDirectory.empty()) {
        logDirectory = Utils::generateLogDirectory();
	}
    std::filesystem::path dirPath(directory);
    if (!std::filesystem::exists(dirPath)) {
        if (!std::filesystem::create_directories(dirPath)) {
            throw std::runtime_error("Cannot create log directory");
        }
    }
}

std::string Logger::formatString(const char* format, ...) {
    va_list args;
    va_start(args, format);
    size_t size = std::vsnprintf(nullptr, 0, format, args) + 1;
    std::unique_ptr<char[]> buf(new char[size]);
    std::vsnprintf(buf.get(), size, format, args);
    va_end(args);
    return std::string(buf.get(), buf.get() + size - 1);
}