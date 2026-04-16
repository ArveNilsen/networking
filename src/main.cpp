#include <networking/Init.hpp>

#include <networking/Socket.hpp>

#include <print>
#include <span>
#include <string>

int main() {
    Init init;

    std::println("{}", "Creating socket...");
    Socket socket;

    std::println("{}", "Binding socket to address...");
    auto ec = socket.bind("127.0.0.1");
    if (ec.value() != 0)
        std::println("{}", ec.message());

    std::println("{}", "Listening...");
    ec = socket.listen(3); 
    if (ec.value() != 0)
        std::println("{}", ec.message());

    std::println("{}", "Waiting for connection...");
    Socket client = socket.accept();

    std::println("{}", "Client is connected...");

    std::println("{}", "Reading request...");
    char request[1024];
    std::size_t bytes_received = client.recv(std::as_writable_bytes(std::span(request)));
    std::println("Bytes received: {}", bytes_received);

    
    std::println("{}", "Sending response...");

    const std::string response = R"(HTTP/1.1 200 OK\r\n
                                    Connection: close\r\n
                                    Content-Type: text/plain\r\n\r\n
                                    Local time is: )";

    std::size_t bytes_sent = client.send(std::as_bytes(std::span(response)));
    std::println("Sent {} of {} bytes", bytes_sent, response.length());


    return 0;
}
