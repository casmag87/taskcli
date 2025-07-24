#include "CommandParser.h"
#include <sstream>

ParsedCommand CommandParser::parse(const std::string& input) {
    ParsedCommand cmd;
    std::istringstream iss(input);
    std::string token;

    while (iss >> token) {
        if (token.size() >= 2 && token.compare(0, 2, "--") == 0) {
            std::string key = token.substr(2);
            std::string val;
            iss >> val;
            cmd.options[key] = val;
        } else {
            cmd.tokens.push_back(token);
        }
    }

    if (!cmd.tokens.empty()){
        cmd.action = cmd.tokens[0];
    }

    return cmd;
}




