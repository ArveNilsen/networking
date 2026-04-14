#include <networking/Init.hpp>

Init::Init() {
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        throw std::runtime_error("Failed to initialize.");
    }
}

Init::~Init() {
    WSACleanup();
}
