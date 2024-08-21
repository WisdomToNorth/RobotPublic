#ifndef RFSPAN_H
#define RFSPAN_H

#include <cassert>
#include <climits>
#include <span>

template <typename T>
using Span = std::span<T>;

// Returns the index of 'item' contained in 'span'
template <typename T, typename U>
constexpr auto spanItemIndex(std::span<T> span, U &&item) ->
    typename std::enable_if<std::is_same<T, std::decay_t<U>>::value, int>::type
{
    assert(!span.empty());
    auto index = &item - &span.front();
    assert(index >= 0);
    assert(index <= INT_MAX);
    assert(&span[static_cast<typename std::span<T>::size_type>(index)] == &item);
    return static_cast<int>(index);
};

template <typename Container>
constexpr int cSpanItemIndex(Container &cont, typename Container::const_reference item)
{
    return spanItemIndex(std::span<typename Container::value_type>(cont), item);
};
#endif
