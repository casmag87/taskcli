#include "TaskClient.h"
#include <arpa/inet.h>   // For inet_pton, sockaddr_in
#include <unistd.h>      // For close()
#include <iostream>      // For std::cout, std::cerr

// Constructor: Initializes host, port, and sets socket to invalid (-1)
TaskClient::TaskClient(const std::string& host, int port)
    : host(host), port(port), sock(-1) {}

// Connects to the server using TCP sockets
bool TaskClient::connectToServer() {
    sock = socket(AF_INET, SOCK_STREAM, 0);  // Create a TCP socket
    if (sock < 0) return false;              // Return false on failure

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;         // IPv4
    serverAddr.sin_port = htons(port);       // Convert port to network byte order
    inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr);  // Convert host to binary form

    // Try connecting to the server
    return connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) >= 0;
}

// Sends a command to the server and receives the response
std::string TaskClient::sendCommand(const std::string& command) {
    if (sock < 0) return "Not connected.";  // Ensure connection exists

    std::string cmd = command + "\n";       // Append newline to signal end of command
    send(sock, cmd.c_str(), cmd.size(), 0); // Send the command to the server

    char buffer[4096];                      // Buffer to store the response
    ssize_t received = recv(sock, buffer, sizeof(buffer) - 1, 0); // Read server response
    if (received <= 0) return "Connection error."; // Return error message on failure

    buffer[received] = '\0';                // Null-terminate received data
    return std::string(buffer);             // Return as std::string
}

// Closes the socket and resets internal state
void TaskClient::disconnect() {
    if (sock >= 0) close(sock);             // Close the socket if open
    sock = -1;                              // Reset socket to invalid state
}
// Destructor: Ensures the socket is closed if still open
TaskClient::~TaskClient() {
    disconnect();                           // Call disconnect to clean up
}

