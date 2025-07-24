#pragma once 

#include <string>
#include <chrono>

enum class Role {
    USER,
    ADMIN

};


class User {
public:
    std::string username;
    std::string hashedToken;
    Role role;
    std::chrono::system_clock::time_point expiry;

    User() = default;
    User(const std::string& username, const std::string& hashedToken, Role role, const std::chrono::system_clock::time_point& expiry);
    
    bool isExpired() const;
    std::string roleToString() const;
    static Role stringToRole(const std::string& roleStr);





};
