#include "logger.hpp"

Logger::Logger() : logPath("journal.log")
{
    std::ifstream in(logPath);
    std::string line;
    while (std::getline(in, line))
    {
        if (!line.empty())
            entries.push_back(line);
    }
    in.close();

    fileStream.open(logPath, std::ios::app);
}

Logger::~Logger()
{
    if (fileStream.is_open())
        fileStream.close();
}

Logger &Logger::getInstance()
{
    static Logger logger;
    return logger;
}

const char *Logger::levelName(Level level) noexcept
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

std::string Logger::timestamp()
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

void Logger::log(Level level, const std::string &message)
{
    std::ostringstream line;
    line << "[" << timestamp() << "] [" << levelName(level) << "] " << message;

    entries.push_back(line.str());

    if (fileStream.is_open())
    {
        fileStream << line.str() << '\n';
        fileStream.flush();
    }
    else
    {
        std::cerr << "[" << timestamp() << "] [ERROR] Could not write to journal file '" << logPath << "'\n";
    }

    if (level == Level::Error)
        std::cerr << line.str() << '\n';
    else
        std::cout << line.str() << '\n';
}

void Logger::info(const std::string &message)
{
    log(Level::Info, message);
}

void Logger::warning(const std::string &message)
{
    log(Level::Warning, message);
}

void Logger::error(const std::string &message)
{
    log(Level::Error, message);
}

void Logger::debug(const std::string &message)
{
    log(Level::Debug, message);
}

const std::vector<std::string> &Logger::getEntries() const noexcept
{
    return entries;
}

void Logger::clear()
{
    entries.clear();
    if (fileStream.is_open())
        fileStream.close();
    fileStream.open(logPath, std::ios::trunc);
}
