#pragma once

#include <atomic>
#include <iostream>
#include <string>

class IObject
{
private:
    inline static std::atomic<int> contor{0};
    const int id;

protected:
    IObject() : id(++contor) {}
    IObject(const IObject &) : id(++contor) {}

public:
    virtual ~IObject() = default;

    int getId() const noexcept
    {
        return id;
    }

    virtual std::string toString() const = 0;

    friend std::ostream &operator<<(std::ostream &os, const IObject &obj)
    {
        return os << obj.toString();
    }

    bool operator==(const IObject &alt) const noexcept
    {
        return id == alt.id;
    }
};
