#pragma once

#include <cstddef>
#include <iterator>
#include <functional>

template <typename T, typename Next>
class ListIter {
public:
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T*;
    using reference         = T&;
    using iterator_category = std::forward_iterator_tag;

    ListIter() : current_(nullptr) {}
    explicit ListIter(T* current) : current_(current) {}

    // Prefix
    ListIter& operator++() {
        if (current_)
            current_ = Next{}(current_);

        return *this;
    }

    // Postfix
    ListIter operator++(int) {
        ListIter temp(*this);
        if (current_)
            current_ = Next{}(current_);

        return temp;
    }

    reference operator*() const {
        return *current_;
    }

    pointer operator->() const {
        return &(operator*());
    }

    bool operator==(const ListIter& other) const {
        return current_ == other.current_;
    }

    bool operator!=(const ListIter& other) const {
        return !(*this == other);
    }

    T* current_;
};

