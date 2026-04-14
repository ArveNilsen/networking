#pragma once

#include <cstddef>
#include <iterator>

/*
 * Next needs to be default constructible, callable, takes T* and return something assignable to T*
 */

template <typename T, typename Next>
class ListIter {
public:
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T*;
    using reference         = T&;
    using iterator_concept  = std::forward_iterator_tag;

    constexpr ListIter() : current_(nullptr) {}
    constexpr explicit ListIter(T* current) : current_(current) {}

    // Prefix
    constexpr ListIter& operator++() {
        if (current_)
            current_ = Next{}(current_);

        return *this;
    }

    // Postfix
    constexpr ListIter operator++(int) {
        ListIter temp(*this);
        if (current_)
            current_ = Next{}(current_);

        return temp;
    }

    constexpr reference operator*() const {
        return *current_;
    }

    constexpr pointer operator->() const {
        return current_;
    }

    constexpr bool operator==(const ListIter& other) const {
        return current_ == other.current_;
    }

    constexpr bool operator!=(const ListIter& other) const {
        return !(*this == other);
    }

    T* current_;
};

