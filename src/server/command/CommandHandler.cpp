#include "CommandHandler.h"
#include "Manual.h"
#include <sstream>
#include "Alias_resolver.h"

// Constructor: Initializes command map with supported commands and their handlers
CommandHandler::CommandHandler(TaskManager& manager) : manager(manager) {
    commandMap = {
        { "add",  [this](const ParsedCommand& cmd) { return handleAdd(cmd); } },
        { "list", [this](const ParsedCommand&)     { return handleList(); } },
        { "show", [this](const ParsedCommand& cmd) { return handleShow(cmd); } },
        { "help", [this](const ParsedCommand&)     { return printManual(); } },
        { "man",  [this](const ParsedCommand&)     { return printManual(); } }
    };
}

// Parses a string into a Priority enum value
Priority CommandHandler::parsePriority(const std::string& str) {
    if (str == "low") return Priority::Low;
    if (str == "high") return Priority::High;
    return Priority::Medium; // Default
}

// Parses a string into a Status enum value
Status CommandHandler::parseStatus(const std::string& str) {
    if (str == "open") return Status::Open;
    if (str == "in-progress") return Status::InProgress;
    return Status::Closed; // Default
}

// Converts a Status enum value to its string representation
std::string CommandHandler::statusToString(Status s) {
    switch (s) {
        case Status::Open: return "open";
        case Status::InProgress: return "in-progress";
        case Status::Closed: return "closed";
        case Status::None: return "none";  
    }
    return "";
}

// Converts a Priority enum value to its string representation
std::string CommandHandler::priorityToString(Priority p) {
    switch (p) {
        case Priority::Low: return "low";
        case Priority::Medium: return "medium";
        case Priority::High: return "high";
        case Priority::None: return "none"; 
    }
    return "";
}

// Executes a parsed command by looking it up in the command map
std::string CommandHandler::execute(const ParsedCommand& cmd) {
    const std::string action = resolveAlias(cmd.action); // Handle command aliasing
    auto it = commandMap.find(action);

    if (it != commandMap.end()) {
        return it->second(cmd); // Call corresponding handler
    }

    return "Unknown or unsupported command";
}

// Handles the "add" command: creates a task from user input
std::string CommandHandler::handleAdd(const ParsedCommand& cmd) {
    Task t;

    // Extract task title from remaining tokens
    if (cmd.tokens.size() > 1) {
        std::ostringstream titleStream;
        for (size_t i = 1; i < cmd.tokens.size(); ++i) {
            if (i > 1) titleStream << " ";
            titleStream << cmd.tokens[i];
        }
        t.title = titleStream.str();
    } else {
        t.title = "Untitled"; // Default title
    }

    // Set optional parameters if provided
    if (cmd.options.count("desc"))     t.description = cmd.options.at("desc");
    if (cmd.options.count("priority")) t.priority    = parsePriority(cmd.options.at("priority"));
    if (cmd.options.count("due"))      t.dueDate     = cmd.options.at("due");

    int id = manager.addTask(t); // Add task and retrieve new task ID
    return "Task added with ID: " + std::to_string(id);
}

// Handles the "list" command: returns a summary of all tasks
std::string CommandHandler::handleList() {
    std::ostringstream oss;
    for (const auto& t : manager.listTasks()) {
        oss << "#" << t.id << ": " << t.title << " (" << statusToString(t.status) << ")\n";
    }
    return oss.str();
}

// Handles the "show" command: displays full details of a specific task
std::string CommandHandler::handleShow(const ParsedCommand& cmd) {
    if (cmd.tokens.size() < 2) {
        return "Usage: show <task_id>"; // Command usage hint
    }

    try {
        int id = std::stoi(cmd.tokens[1]);        // Parse task ID
        auto t = manager.getTask(id);             // Retrieve task
        if (!t) return "Task not found";

        // Format task details
        std::ostringstream oss;
        oss << "ID: " << t->id << "\nTitle: " << t->title
            << "\nDesc: " << t->description
            << "\nPriority: " << priorityToString(t->priority)
            << "\nDue: " << t->dueDate
            << "\nAssigned: " << t->assignedUser
            << "\nStatus: " << statusToString(t->status);
        return oss.str();
    } catch (...) {
        return "Invalid task ID format"; // Catch conversion errors
    }
}

