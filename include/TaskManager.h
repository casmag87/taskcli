#pragma once 
#include "Task.h"
#include <vector>
#include <mutex>
#include <optional>


class TaskManager {
public:
   TaskManager();
   int addTask(const Task& task);
   std::optional<Task> getTask(int id);
   bool updateTask(int id, const Task& updated);
   
   std::vector<Task> listTasks(std::optional<Status> status = std::nullopt);

   bool assignTask(int id, const std::string& user);
   bool closeTask(int id);
   bool reopenTask(int id);
   bool deleteTask(int id);  

   void saveToFile(const std::string& filename) const; 
   void loadFromFile(const std::string& filename);

   void saveToFile() const { saveToFile(filename); }
   void loadFromFile() { loadFromFile(filename); }


private:
   std::vector<Task> tasks;
   int nextId;
   std::mutex mtx;
   const std::string filename = "tasks.json";  

};
