#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

constexpr int PORT = 8080;

std::string http_response = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 13\r\n"
    "\r\n"
    "Hello, world!";

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address{};
    socklen_t addrlen = sizeof(address);

    // create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        return 1;
    }

    // address config
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
     
    // bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }

    // connect
    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        return 1;
    }

    std::cout << "Server started on port " << PORT << std::endl;

    while(true) {
        client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        char buffer[1024] = {0};
        read(client_fd, buffer, sizeof(buffer));
        std::cout << "Received request:\n" << buffer << std::endl;

        send(client_fd, http_response.c_str(), http_response.size(), 0);
        close(client_fd);
    }

    return 0;
}
