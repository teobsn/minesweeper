#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

template<typename T>
T clampValue(const T &value, const T &low, const T &high)
{
    if (value < low)
        return low;
    if (high < value)
        return high;
    return value;
}

template<typename T>
const T *findBest(const std::vector<T> &values)
{
    if (values.empty())
        return nullptr;

    return &*std::max_element(values.begin(), values.end());
}

template<typename T, std::size_t Capacity>
class StaticQueue
{
private:
    std::unique_ptr<T[]> data;
    std::size_t count;

public:
    StaticQueue() : data(new T[Capacity]), count(0)
    {
    }

    StaticQueue(const StaticQueue &other) : data(new T[Capacity]), count(other.count)
    {
        for (std::size_t i = 0; i < count; ++i)
            data[i] = other.data[i];
    }

    StaticQueue &operator=(const StaticQueue &other)
    {
        if (this == &other)
            return *this;

        count = other.count;
        for (std::size_t i = 0; i < count; ++i)
            data[i] = other.data[i];

        return *this;
    }

    ~StaticQueue() noexcept = default;

    void push(const T &value)
    {
        if (count >= Capacity)
            throw std::overflow_error("StaticQueue este plina");

        data[count++] = value;
    }

    void pop()
    {
        if (count == 0)
            throw std::underflow_error("StaticQueue este goala");

        for (std::size_t i = 1; i < count; ++i)
            data[i - 1] = data[i];
        --count;
    }

    void clear() noexcept
    {
        count = 0;
    }

    std::size_t size() const noexcept
    {
        return count;
    }

    bool empty() const noexcept
    {
        return count == 0;
    }

    bool full() const noexcept
    {
        return count == Capacity;
    }

    const T &operator[](std::size_t index) const
    {
        return data[index];
    }

    T &operator[](std::size_t index)
    {
        return data[index];
    }

    const T *begin() const noexcept
    {
        return data.get();
    }

    const T *end() const noexcept
    {
        return data.get() + count;
    }

    T *begin() noexcept
    {
        return data.get();
    }

    T *end() noexcept
    {
        return data.get() + count;
    }

    template<typename U, typename Mapper>
    StaticQueue<U, Capacity> transform(Mapper mapper) const
    {
        StaticQueue<U, Capacity> result;
        for (std::size_t i = 0; i < count; ++i)
            result.push(mapper(data[i]));
        return result;
    }
};
