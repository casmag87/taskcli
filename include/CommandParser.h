#pragma once 
#include <string>
#include <map>
#include <vector>

struct ParsedCommand {
    std::string action;
    std::vector<std::string> tokens;
    std::map<std::string, std::string> options;
};

class CommandParser{
public:
    ParsedCommand parse(const std::string& input);

};


