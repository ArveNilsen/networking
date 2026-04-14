#include <networking/SocketPosixPolicy.hpp>

// C headers
#include <cstddef>
#include <cstdint>
#include <cstring>

// Platform headers
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace platform {
int SocketPosixPolicy::open() {
    return ::socket(
            AF_INET,        // int domain
            SOCK_STREAM,    // int type
            0               // int protocol
    );
}

void SocketPosixPolicy::close(int fd) {
    if (fd >= 0) ::close(fd);
}

std::error_code SocketPosixPolicy::connect(int fd, std::string_view addr) {
    // TODO: Implement connect
    (void)fd;
    (void)addr;
    return {};
}

std::error_code SocketPosixPolicy::bind(int fd, std::string_view addr) {
    sockaddr_in sa {};
    sa.sin_family = AF_INET;
    sa.sin_port = htons(8080); // TODO: This should not be hard coded!

    if (::inet_pton(AF_INET, addr.data(), &sa.sin_addr) != 1)
        return std::make_error_code(std::errc::address_not_available);

    if (::bind(fd, reinterpret_cast<sockaddr*>(&sa), sizeof(sa)) != 0)
        return std::error_code{errno, std::generic_category()};

    return {};
}

std::size_t SocketPosixPolicy::send(int fd, std::span<const std::byte> buf) {
    return static_cast<std::size_t>(::send(fd, buf.data(), buf.size(), 0));
}

std::size_t SocketPosixPolicy::recv(int fd, std::span<std::byte> buf) {
    ssize_t n = ::recv(fd, buf.data(), buf.size(), 0);
    return n < 0 ? 0 : static_cast<std::size_t>(n);
}

} // namespace platform
