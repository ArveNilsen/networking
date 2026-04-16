#pragma once
#include <concepts>
#include <string_view>
#include <system_error>
#include <span>

#include <networking/SocketDefaultPolicy.hpp>

// Concept descibing the required interface.
// This models what is expected of the underlying socket impl
template <typename P>
concept SocketPolicy = requires(P p, int fd, std::string_view addr, 
        std::span<const std::byte> buf) {
    { P::open() } -> std::convertible_to<int>; // returns native handle or -1
    { P::close(std::declval<int>()) } -> std::same_as<void>; // close returns void
    { P::connect(std::declval<int>(), addr) } ->
        std::same_as<std::error_code>;
    { P::send(std::declval<int>(), buf) } -> std::same_as<std::size_t>;
    { P::recv(std::declval<int>(), std::declval<std::span<std::byte>>()) } 
        -> std::same_as<std::size_t>;
};

// DefaultPolicy must be defined by the build system (see CMakeLists)
#if 0
#ifndef DEFAULT_SOCKET_POLICY
#error "DEFAULT_SOCKET_POLICY must be defined by the build system."
#endif

// Forward declare the policy type chosen at build-time.
namespace platform {
struct DEFAULT_SOCKET_POLICY;
}
#endif
using DefaultPolicy = platform::DEFAULT_SOCKET_POLICY;

// Adapter that forwards to policy implementation.
template <SocketPolicy Policy = DefaultPolicy>
class Socket {
public:
    Socket() : fd_(Policy::open()) {
    if (fd_ < 0)
        throw std::system_error(std::make_error_code(std::errc::io_error));
    }

    ~Socket() { if (fd_ >= 0) Policy::close(fd_); }

    std::error_code connect(std::string_view addr) {
        return Policy::connect(fd_, addr);
    }

    std::error_code bind(std::string_view addr) {
        return Policy::bind(fd_, addr);
    }

    std::size_t send(std::span<const std::byte> buf) {
        return Policy::send(fd_, buf);
    }

    std::size_t recv(std::span<std::byte> buf) {
        return Policy::recv(fd_, buf);
    }

    std::error_code listen(int backlog) {
        return Policy::listen(fd_, backlog);
    }

    // This can return an invalid socket
    // Consider returning a struct or pair with fd and errno
    Socket accept() {
        return Socket(Policy::accept(fd_));
    }

    int native_handle() const noexcept { return fd_; }

    // Non-copyable, moveable
    Socket(const Socket&)               = delete;
    Socket& operator=(const Socket&)    = delete;
    Socket(Socket&& other) noexcept : fd_(other.fd_) { other.fd_ = -1; }
    Socket& operator=(Socket&& other) noexcept {
        if (this != &other) {
            if (fd_ >= 0) 
                Policy::close(fd_);

            fd_ = other.fd_;
            other.fd_ = -1;
        }

        return *this;
    }
            

private:
    explicit Socket(int fd) : fd_(fd) {}
    int fd_{-1};
};
