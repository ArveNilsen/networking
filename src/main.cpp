#include <string>
#include <print>
#include <exception>
#include <ranges>
#include <memory>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>

struct NetworkAdapterInfo { // Network Adapter Information
    std::string interfaceName_, addressFamily, ipAddress;

    NetworkAdapterInfo() = default;
    NetworkAdapterInfo(const char* i, const char* f, const char *n)
        : interfaceName_(i), addressFamily(f), ipAddress(n) {}
};

template <>
struct std::formatter<NetworkAdapterInfo> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin(); 
    }

    auto format(const NetworkAdapterInfo& n, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}\t{}\t\t{}", n.interfaceName_, n.addressFamily, n.ipAddress);
    }
};

class ListIterator {
public:
    using difference_type   = std::ptrdiff_t;
    using value_type        = ifaddrs;
    using pointer           = ifaddrs*;
    using reference         = ifaddrs&;
    using iterator_category = std::forward_iterator_tag;

    ListIterator() = default;
    explicit ListIterator(ifaddrs *i) 
        : current_(i) {}

    // Prefix
    ListIterator& operator++() {
        if (current_)
            current_ = current_->ifa_next;

        return *this;
    }

    // Postfix
    ListIterator operator++(int) {
        ListIterator temp(*this);
        if (current_)
            current_ = current_->ifa_next;

        return temp;
    }

    reference operator*() const {
        return *current_;
    }

    pointer operator->() const {
        return &(operator*());
    }

    bool operator==(const ListIterator& other) const {
        return current_ == other.current_;
    }

    bool operator!=(const ListIterator& other) const {
        return !(*this == other);
    }

private:
    value_type *current_;
};

class IfAddrs {
public:
    static constexpr auto deleter = [](ifaddrs *ia) {
        if (ia)
            freeifaddrs(ia);
    };

    explicit IfAddrs() : addr_list_(nullptr) {
        struct ifaddrs *addresses; // TODO: Add RAII for this type

        if (getifaddrs(&addresses) == -1) {
            throw std::runtime_error("getifaddrs call failed");
        }

        addr_list_.reset(addresses);
    }

    ListIterator begin() const {
        return ListIterator(addr_list_.get());
    }

    ListIterator end() const {
        return ListIterator(nullptr);
    }

private:
    std::unique_ptr<ifaddrs, decltype(deleter)> addr_list_;
};


int main() {
    static_assert(std::ranges::range<IfAddrs>);
    constexpr auto string_repr = [](ifaddrs &ia) {
        NetworkAdapterInfo networkAdapterInfo;
        networkAdapterInfo.interfaceName_ = ia.ifa_name;
        int family = ia.ifa_addr->sa_family;
        networkAdapterInfo.addressFamily = family == AF_INET ? "IPv4" : "IPv6";
        switch (ia.ifa_addr->sa_family) {
            case AF_INET: {
                auto address = reinterpret_cast<sockaddr_in *>(ia.ifa_addr);
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(address->sin_addr), ip, INET_ADDRSTRLEN);
                networkAdapterInfo.ipAddress = ip;
                break;
            }
            case AF_INET6: {
                auto address = reinterpret_cast<sockaddr_in6 *>(ia.ifa_addr);
                char ip[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, &(address->sin6_addr), ip, INET6_ADDRSTRLEN);
                networkAdapterInfo.ipAddress = ip;
                break;
            }
            default: {
                networkAdapterInfo.ipAddress = "Unknown";
                break;
            }
        }
        
        return networkAdapterInfo;
    };

    auto ips = std::views::all(IfAddrs{}) 
        | std::views::filter([](const auto& ia) {
            int family = ia.ifa_addr->sa_family;
            return (family == AF_INET || family == AF_INET6);
        })
        | std::views::transform([&string_repr](auto& ia){ return string_repr(ia); });

    for (const auto& ip : ips) {
        std::println("{}", ip);
    }
    
    return 0;
}
