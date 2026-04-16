#include <networking/SocketPosixPolicy.hpp>

// C headers
#include <cstddef>
#include <cstdint>
#include <cstring>

// Platform headers
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <print>
#include <cerrno>

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
    std::println("Send buf size: {}", buf.size());
    return static_cast<std::size_t>(::send(fd, buf.data(), buf.size(), 0));
}

std::size_t SocketPosixPolicy::recv(int fd, std::span<std::byte> buf) {
    std::println("Recv buf size: {}", buf.size());
    ssize_t n = ::recv(fd, buf.data(), buf.size(), 0);
    if (n == -1)
        std::println("errno {} {}", errno, std::strerror(errno));
    return n < 0 ? 0 : static_cast<std::size_t>(n);
}

std::error_code SocketPosixPolicy::listen(int fd, int backlog) {
    if (::listen(fd, backlog) != 0)
        return std::error_code{errno, std::generic_category()};

    return {};
}

int SocketPosixPolicy::accept(int fd) {
    sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    return ::accept(fd, (struct sockaddr*) &client_address, &client_len);
}
    
} // namespace platform
