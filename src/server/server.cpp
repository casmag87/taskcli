#include "Server.h"
#include "TaskClient.h"
#include "CommandParser.h"
#include "CommandHandler.h"
#include "TaskManager.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Logger.h"

///Constructor Inialize server with specified port 
// Constructor implementation with proper member initialization order
Server::Server(int port)
    : taskManager()          // 1
    , parser()               // 2
    , handler(taskManager)   // 3
    , port(port)             // 4
    , server_socket_fd(-1)   // 5
    , running(false)         // 6
    , clients()
    , clients_mutex()
    , accept_thread()
{
    // constructor body if you want
}


///Deconstructor: Stop the server and clean up
Server::~Server(){
   stop();
}

// starts the server and listning to client connections
void Server::start() {
   taskManager.loadFromFile("tasks.json");
   server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
   if (server_socket_fd <0){
       
       return;
   }

   //Allow socket address reuse (avoids Adress already in error on restart
   int opt =1;
   setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
   
   //Prepare server address structure 
   sockaddr_in server_addr{};
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = INADDR_ANY; //Listen on all interfaces
   server_addr.sin_port = htons(port); // Host to network byte order 

   //Bind the socket to the port
   if (bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) <0){
       
       return;
   }

   //start listening for connections
   if(listen(server_socket_fd, SOMAXCONN) <0){
      
      return;
   }

    std::cout << "Server started on port " << port << "\n";
   running = true;

   accept_thread = std::thread(&Server::acceptClients,this);

}

void Server::stop() {
    if(!running) return;

    running = false;

    // Close the listning 
    close(server_socket_fd);

    // Join the acepting thread if it is running 
    if (accept_thread.joinable())
        accept_thread.join();

   // Lock the clients list and close all 
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (auto& client : clients){
        close(client->socket_fd);
    } 
    clients.clear();

    

}

// Accepts new clients and spawns a thread for each
void Server::acceptClients() {
    while (running) {
	sockaddr_in client_addr{};
	socklen_t client_len = sizeof(client_addr);
	
	// Accept a new connection 
        int client_fd = accept(server_socket_fd, (struct sockaddr*)&client_addr, &client_len);
        if(client_fd <0){
	   if (running) //Dont print error if shutting down
	      
           continue;
	}

	//Generate a simple client ID (e.g., IP:PORT)
        std::string client_id = inet_ntoa(client_addr.sin_addr) + std::string(":") + std::to_string(ntohs(client_addr.sin_port));
    
       
		

       //Wrap client in a shared_ptr for lifetime management
       auto client = std::make_shared<Client>(client_fd, client_id);

       // add client to the list (thread-safe)
       {
	    std::lock_guard<std::mutex> lock(clients_mutex);
	    clients.push_back(client);
       }

       std::thread(&Server::handleClient, this, client).detach();
    
    }
}

// Handles communication with a single client
void Server::handleClient(std::shared_ptr<Client> client) {
    char buffer[1024];

    while (running) {
        memset(buffer, 0, sizeof(buffer));

        // Receive data from client
        ssize_t bytes_received = recv(client->socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            std::cout << "Client disconnected: " << client->id << "\n";
            break;
        }

        std::string message(buffer);
        message.erase(message.find_last_not_of(" \r\n") + 1); // Trim newline, spaces
        std::cout << "[" << client->id << "] " << message << "\n";

        // Handle task command
        ParsedCommand cmd = parser.parse(message);
        std::string response = handler.execute(cmd);
        response += "\n";

        send(client->socket_fd, response.c_str(), response.size(), 0);
    }

    // Cleanup
    close(client->socket_fd);
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(std::remove_if(clients.begin(), clients.end(),
                                     [&](const std::shared_ptr<Client>& c) {
                                         return c->socket_fd == client->socket_fd;
                                     }),
                      clients.end());
    }
}
