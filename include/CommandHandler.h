#pragma once
#include "CommandParser.h"
#include "TaskManager.h"


class CommandHandler{
public:
   CommandHandler(TaskManager& manager);
   std::string execute(const ParsedCommand& cmd);
   

private:
   TaskManager& manager; 
   Priority parsePriority(const std::string& str);
   Status parseStatus(const std::string& str);
   std::string statusToString(Status);
   std::string priorityToString(Priority priority);

   std::string handleAdd(const ParsedCommand& cmd);
   std::string handleList();
   std::string handleShow(const ParsedCommand& cmd);
   
   std::unordered_map<std::string, std::function<std::string(const ParsedCommand&)>> commandMap; 

};
