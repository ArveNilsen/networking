#pragma once

#include "ListIter.hpp"

#include <ifaddrs.h>
#include <netdb.h>
#include <exception>
#include <memory>

class IfAddrs {
public:
    static constexpr auto deleter = [](ifaddrs *ia) -> void {
        if (ia)
            freeifaddrs(ia);
    };

    static constexpr auto nextInList = [](ifaddrs *ia) -> ifaddrs* { return ia->ifa_next; };

    explicit IfAddrs() : addr_list_(nullptr) {
        ifaddrs *addresses;

        if (getifaddrs(&addresses) == -1) {
            throw std::runtime_error("getifaddrs call failed");
        }

        addr_list_.reset(addresses);
    }

    ListIter<ifaddrs, decltype(nextInList)> begin() const {
        return ListIter<ifaddrs, decltype(nextInList)>(addr_list_.get());
    }

    ListIter<ifaddrs, decltype(nextInList)> end() const {
        return ListIter<ifaddrs, decltype(nextInList)>(nullptr);
    }

private:
    std::unique_ptr<ifaddrs, decltype(deleter)> addr_list_;
};

