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
    std::vector<std::string> entries;
    std::ofstream fileStream;

    Logger();

    static const char *levelName(Level level) noexcept;
    static std::string timestamp();

public:
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
    ~Logger();

    static Logger &getInstance();

    void log(Level level, const std::string &message);
    void info(const std::string &message);
    void warning(const std::string &message);
    void error(const std::string &message);
    void debug(const std::string &message);

    const std::vector<std::string> &getEntries() const noexcept;
    void clear();
};
