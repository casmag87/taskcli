#pragma once 
#include "User.h"
#include <unordere_map>
#include <string>

class UserManager {
private:
    std::unordered_map<std::string, User> users;
    std::string dataFile;


public:
    explicit UserManager(const std::string& file);
    bool loadUsers();
    bool saveUsers();
    
    bool validateToken(const std::string& username, const std::string& token) const;
    bool addUser(const std::string& username, const std::string& token, Role role, int daysValid)
    bool deleteUser(const std::string& username);

    const User* getUser(const std::string& username) const;    



};
