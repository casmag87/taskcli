#pragma once 
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include "TaskManager.h"
#include "CommandParser.h"
#include "CommandHandler.h"
struct Client {
    int socket_fd;
    std::string id;
    Client(int fd, const std::string& id_) : socket_fd(fd), id(id_) {}
};


class Server {

public:
    explicit Server(int port);
    ~Server();  
    void start();
    void stop();
    

private:
    void acceptClients();
    void handleClient(std::shared_ptr<Client> client);

    TaskManager taskManager;
    CommandParser parser;
    CommandHandler handler;



    int port;
    int server_socket_fd;
    bool running;
    std::vector<std::shared_ptr<Client>> clients;
    std::mutex clients_mutex;
    std::thread accept_thread;

};
