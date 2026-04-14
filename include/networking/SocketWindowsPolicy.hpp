// include/platform/windows_policy.hpp
#pragma once
#include <string_view>
#include <system_error>
#include <span>

namespace platform {
struct windows_policy {
    static int open();
    static void close(int fd);
    static std::error_code connect(int fd, std::string_view addr);
    static std::size_t send(int fd, std::span<const std::byte> buf);
    static std::size_t recv(int fd, std::span<std::byte> buf);
};
} // namespace platform

