#include <networking/ListIter.hpp>

#include <ifaddrs.h>

constexpr bool test_ListIter_is_default_constructible() {
    return std::is_default_constructible_v<ListIter<ifaddrs, decltype([](auto &ia){ia->ifa_next;})>>;
}

int main() {
    static_assert(test_ListIter_is_default_constructible() == true);
}
