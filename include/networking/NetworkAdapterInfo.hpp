#pragma once

#include <string>
#include <format>


#include "IfAddrs.hpp"

namespace nw {

struct NetworkAdapterInfo { // Network Adapter Information
    std::string interfaceName_, addressFamily, ipAddress;

    NetworkAdapterInfo() = default;
    NetworkAdapterInfo(const char* i, const char* f, const char *n)
        : interfaceName_(i), addressFamily(f), ipAddress(n) {}
};

void listNetworkAdapterInfo();

} // namespace nw

template <>
struct std::formatter<nw::NetworkAdapterInfo> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin(); 
    }

    auto format(const nw::NetworkAdapterInfo& n, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}\t{}\t\t{}", n.interfaceName_, n.addressFamily, n.ipAddress);
    }
};

