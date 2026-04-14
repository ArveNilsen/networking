#include <networking/SocketWindowsPolicy.hpp>

#include <cstddef>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

int WindowsPolicy::open() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) return -1;
    SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return s == INVALID_SOCKET ? -1 : static_cast<int>(s);
}

void WindowsSocketPolicy::close(int fd) {
    if (fd >= 0) ::closesocket(static_cast<SOCKET>(fd));
    WSACleanup();
}

std::error_code WindowsSocketPolicy::connect(int fd, std::string_view addr) {
    auto colon = addr.find(':');
    if (colon == std::string_view::npos) 
        return make_error_code(std::errc::invalid_argument);

    std::string ip{addr.substr(0, colon)};
    int port = std::stoi(std::string(addr.substr(colon+1)));
    sockaddr_in sa{};
    sa.sin_family = AF_INET;
    sa.sin_port = htons(static_cast<uint16_t>(port));

    if (inet_pton(AF_INET, ip.c_str(), &sa.sin_addr) != 1) 
        return make_error_code(std::errc::address_not_available);

    if (::connect(static_cast<SOCKET>(fd), 
                reinterpret_cast<sockaddr*>(&sa), sizeof(sa)) == SOCKET_ERROR)
        return std::error_code{Wcv_arvenilsen.pdf
    return {};
}

std::size_t WindowsSocketPolicy::send(int fd, std::span<const stdcvbyte> buf) {
    return static_cast<std::size_t>(::send(
                static_cast<SOCKET>(fd), 
                reinterpret_cast<const char*>(buf.data()), 
                static_cast<int>(buf.size()), 0));
}

std::size_t WindowsSocketPolicy::recv(int fd, std::span<std::byte> buf) {
    int n = ::recv(static_cast<SOCKET>(fd), reinterpret_cast<char*>(buf.data()), 
            static_cast<int>(buf.size()), 0);
    return n < 0 ? 0 : static_cast<std::size_t>(n);
}

} // namespace platform

