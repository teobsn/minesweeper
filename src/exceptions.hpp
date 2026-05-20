#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <iostream>

class MinesweeperException : public std::exception
{
protected:
    std::string message;

public:
    explicit MinesweeperException(const std::string &msg) : message(msg) {}
    virtual ~MinesweeperException() noexcept = default;

    virtual const char *what() const noexcept override
    {
        return message.c_str();
    }
};

class InvalidMoveException final : public MinesweeperException
{
private:
    int x, y;

public:
    InvalidMoveException(const std::string &msg, int x, int y)
        : MinesweeperException(msg), x(x), y(y)
    {
        std::ostringstream oss;
        oss << message << " at coordinates (" << x << ", " << y << ")";
        message = oss.str();
    }

    int getX() const { return x; }
    int getY() const { return y; }
};

class GameConfigException final : public MinesweeperException
{
private:
    int width, height, bombs;

public:
    GameConfigException(const std::string &msg, int w, int h, int b)
        : MinesweeperException(msg), width(w), height(h), bombs(b)
    {
        std::ostringstream oss;
        oss << message << " [Map: " << w << "x" << h << ", Bombs: " << b << "]";
        message = oss.str();
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getBombs() const { return bombs; }
};

class DatabaseException final : public MinesweeperException
{
private:
    int sqliteErrorCode;

public:
    DatabaseException(const std::string &msg, int errorCode)
        : MinesweeperException(msg), sqliteErrorCode(errorCode)
    {
        std::ostringstream oss;
        oss << message << " (SQLite error code: " << errorCode << ")";
        message = oss.str();
    }

    int getErrorCode() const { return sqliteErrorCode; }
};

/// stack unwinding
class ResourceGuard final
{
private:
    std::string resourceName;

public:
    explicit ResourceGuard(const std::string &name) : resourceName(name) {}
    ~ResourceGuard() noexcept
    {
        std::cout << "[Stack Unwinding] Releasing resource: " << resourceName << "\n";
    }
};
