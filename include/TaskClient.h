#pragma once
#include <string>


class TaskClient {
public:
    TaskClient(const std::string& host, int port);
    ~TaskClient(); 
    bool connectToServer();
    std::string sendCommand(const std::string& command);
    void disconnect();

private:
    std::string host;
    int port;
    int sock;    


};
