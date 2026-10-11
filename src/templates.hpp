#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

template <typename T>
T clampValue(const T &value, const T &low, const T &high)
{
    if (value < low)
        return low;
    if (high < value)
        return high;
    return value;
}

template <typename T>
const T *findBest(const std::vector<T> &values)
{
    if (values.empty())
        throw std::out_of_range("findBest called on an empty collection");

    return &*std::max_element(values.begin(), values.end());
}

template <typename T>
class Rezultat
{
private:
    bool success;
    // optional pastreaza valoarea doar cand rezultatul este valid; altfel
    // obiectul ramane intr-o stare de eroare fara pointeri sau alocari manuale.
    std::optional<T> value;
    std::string error;

public:
    // wrapper generic de rezultat: tine fie valoarea buna, fie mesajul de eroare,
    // ca sa putem trata fail-urile asteptate fara exceptii
    Rezultat(const T &value) : success(true), value(value), error() {}
    Rezultat(T &&value) : success(true), value(std::move(value)), error() {}

    static Rezultat eroare(const std::string &error)
    {
        Rezultat result;
        result.success = false;
        result.error = error;
        return result;
    }

    static Rezultat eroare(std::string &&error)
    {
        Rezultat result;
        result.success = false;
        result.error = std::move(error);
        return result;
    }

private:
    Rezultat() : success(false), value(std::nullopt), error() {}

public:
    bool ok() const noexcept
    {
        return success;
    }

    const T &get() const
    {
        if (!success || !value.has_value())
            throw std::logic_error(error);
        return *value;
    }

    T &get()
    {
        if (!success || !value.has_value())
            throw std::logic_error(error);
        return *value;
    }

    const std::string &getEroare() const noexcept
    {
        return error;
    }

    template <typename U, typename F>
    Rezultat<U> transforma(F f) const
    {
        if (!success)
            return Rezultat<U>::eroare(error);

        return Rezultat<U>(f(*value));
    }
};

template <typename T, std::size_t Capacity>
class StaticQueue
{
private:
    std::array<T, Capacity> data;
    std::size_t count;

public:
    // Coada cu capacitate fixa prin parametru de template: elementele stau
    // direct in std::array, deci nu folosim new si nu avem realocari.
    // push/pop raman O(1) si structura e potrivita pentru istorii mici si fixe
    StaticQueue() : data{}, count(0)
    {
    }

    StaticQueue(std::initializer_list<T> init) : data{}, count(0)
    {
        if (init.size() > Capacity)
            throw std::overflow_error("StaticQueue init list exceeds capacity");

        for (const T &value : init)
            push(value);
    }

    StaticQueue(const StaticQueue &other) : data(other.data), count(other.count)
    {
    }

    StaticQueue &operator=(const StaticQueue &other) = default;

    ~StaticQueue() noexcept = default;

    void push(const T &value)
    {
        if (count >= Capacity)
            throw std::overflow_error("StaticQueue este plina");

        data[count++] = value;
    }

    void push(T &&value)
    {
        if (count >= Capacity)
            throw std::overflow_error("StaticQueue este plina");

        data[count++] = std::move(value);
    }

    void pop()
    {
        if (count == 0)
            throw std::underflow_error("StaticQueue este goala");

        for (std::size_t i = 1; i < count; ++i)
            data[i - 1] = std::move(data[i]);
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
        return data.data();
    }

    const T *end() const noexcept
    {
        return data.data() + count;
    }

    T *begin() noexcept
    {
        return data.data();
    }

    T *end() noexcept
    {
        return data.data() + count;
    }

    template <typename U, typename Mapper>
    StaticQueue<U, Capacity> transform(Mapper mapper) const
    {
        StaticQueue<U, Capacity> result;
        for (std::size_t i = 0; i < count; ++i)
            result.push(mapper(data[i]));
        return result;
    }
};
