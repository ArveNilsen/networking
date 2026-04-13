#include "NetworkAdapterInfo.hpp"

#include <ranges>
#include <print>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>


void nw::listNetworkAdapterInfo() {
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
    
}

