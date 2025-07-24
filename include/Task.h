#pragma once 
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
enum class Status {None,Open, InProgress, Closed};
enum class Priority {None,Low, Medium, High};

struct Task {
   int id;
   std::string title;
   std::string description;
   Status status =Status::Open;
   Priority priority = Priority::Medium;
   std::string dueDate;
   std::string assignedUser;

   json toJson() const {
        return json{
            {"id", id},
            {"title", title},
            {"description", description},
            {"status", static_cast<int>(status)},
            {"priority", static_cast<int>(priority)},
            {"dueDate", dueDate},
            {"assignedUser", assignedUser}
        };
    }

    static Task fromJson(const json& j) {
        Task task;
        task.id = j.at("id").get<int>();
        task.title = j.at("title").get<std::string>();
        task.description = j.at("description").get<std::string>();
        task.status = static_cast<Status>(j.at("status").get<int>());
        task.priority = static_cast<Priority>(j.at("priority").get<int>());
        task.dueDate = j.at("dueDate").get<std::string>();
        task.assignedUser = j.at("assignedUser").get<std::string>();
        return task;
    }

};

