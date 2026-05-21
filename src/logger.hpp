#pragma once

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Logger final
{
public:
    enum class Level
    {
        Info,
        Warning,
        Error,
        Debug
    };

private:
    std::string logPath;

    Logger();

    static const char *levelName(Level level) noexcept;
    static std::string timestamp();

public:
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    static Logger &getInstance();

    void log(Level level, const std::string &message);
    void info(const std::string &message);
    void warning(const std::string &message);
    void error(const std::string &message);
    void debug(const std::string &message);

    std::vector<std::string> getEntries() const;
    void clear();
    bool empty() const;
    const std::string &getPath() const noexcept;
};

inline Logger::Logger() : logPath("journal.log")
{
}

inline Logger &Logger::getInstance()
{
    static Logger logger;
    return logger;
}

inline const char *Logger::levelName(Level level) noexcept
{
    switch (level)
    {
    case Level::Info:
        return "INFO";
    case Level::Warning:
        return "WARNING";
    case Level::Error:
        return "ERROR";
    case Level::Debug:
        return "DEBUG";
    }

    return "INFO";
}

inline std::string Logger::timestamp()
{
    using clock = std::chrono::system_clock;
    auto now = clock::now();
    std::time_t time = clock::to_time_t(now);
    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &time);
#else
    localtime_r(&time, &tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

inline void Logger::log(Level level, const std::string &message)
{
    std::ofstream out(logPath, std::ios::app);
    if (!out)
    {
        std::cerr << "[" << timestamp() << "] [ERROR] Could not open journal file '" << logPath << "'\n";
        return;
    }

    std::ostringstream line;
    line << "[" << timestamp() << "] [" << levelName(level) << "] " << message;

    out << line.str() << '\n';
    if (!out.good())
    {
        std::cerr << "[" << timestamp() << "] [ERROR] Could not write to journal file '" << logPath << "'\n";
        return;
    }

    if (level == Level::Error)
        std::cerr << line.str() << '\n';
    else
        std::cout << line.str() << '\n';
}

inline void Logger::info(const std::string &message)
{
    log(Level::Info, message);
}

inline void Logger::warning(const std::string &message)
{
    log(Level::Warning, message);
}

inline void Logger::error(const std::string &message)
{
    log(Level::Error, message);
}

inline void Logger::debug(const std::string &message)
{
    log(Level::Debug, message);
}

inline std::vector<std::string> Logger::getEntries() const
{
    std::ifstream in(logPath);
    std::vector<std::string> entries;
    std::string line;
    while (std::getline(in, line))
    {
        if (!line.empty())
            entries.push_back(line);
    }
    return entries;
}

inline void Logger::clear()
{
    std::ofstream out(logPath, std::ios::trunc);
}

inline bool Logger::empty() const
{
    std::ifstream in(logPath);
    return !in.good() || in.peek() == std::ifstream::traits_type::eof();
}

inline const std::string &Logger::getPath() const noexcept
{
    return logPath;
}
