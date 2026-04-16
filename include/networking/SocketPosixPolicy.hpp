#pragma once

// C++ headers
#include <string_view>
#include <system_error>
#include <span>
#include <vector>

namespace platform {

struct SocketPosixPolicy {
    static int open();
    static void close(int fd);
    static std::error_code connect(int fd, std::string_view addr);
    static std::error_code bind(int fd, std::string_view addr);
    static std::size_t send(int fd, std::span<const std::byte> buf);
    static std::size_t recv(int fd, std::span<std::byte> buf);
    static std::error_code listen(int fd, int backlog);
    static int accept(int fd);
};

} // namespace platform
