#include "TaskManager.h"
#include <algorithm>
#include <fstream>

TaskManager::TaskManager() : nextId(1) {}

// Add a new task, assign unique ID, add to tasks vector
int TaskManager::addTask(const Task& task) {
    std::lock_guard<std::mutex> lock(mtx);
    Task t = task;
    t.id = nextId++;
    tasks.push_back(t);  // use 'tasks' here
    saveToFile();
    return t.id;    
}

// Return a copy of the task with matching id or std::nullopt if not found
std::optional<Task> TaskManager::getTask(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& t : tasks) {
        if (t.id == id) return t;
    }
    return std::nullopt;
}

// Update only provided fields
bool TaskManager::updateTask(int id, const Task& updates) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& t : tasks) {
        if (t.id == id) {
            if (!updates.title.empty()) t.title = updates.title;
            if (!updates.description.empty()) t.description = updates.description;
            if (!updates.dueDate.empty()) t.dueDate = updates.dueDate;
            if (updates.priority != Priority::None) t.priority = updates.priority;
            if (updates.status != Status::None) t.status = updates.status;
            return true;
        }
    }
    return false;
}

// Delete the task with matching id
bool TaskManager::deleteTask(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = std::remove_if(tasks.begin(), tasks.end(), [id](const Task& t) {
        return t.id == id;
    });
    if (it != tasks.end()) {
        tasks.erase(it, tasks.end());
        return true;
    }
    return false;
}

// List all tasks or filter by status if provided
std::vector<Task> TaskManager::listTasks(std::optional<Status> status){
    std::lock_guard<std::mutex> lock(mtx);
    if (!status) return tasks;

    std::vector<Task> filtered;
    for (const auto& t : tasks) {
        if (t.status == *status) filtered.push_back(t);
    }
    loadFromFile();
    return filtered;
}

void TaskManager::saveToFile(const std::string& filename) const {
    json j;
    for (const auto& task : tasks){
        j.push_back(task.toJson());

    }    

    std::ofstream file(filename);
    if (file.is_open()){
        file << j.dump(4); 

    }
}

void TaskManager::loadFromFile(const std::string& filename){
    std::ifstream file(filename);
    if (!file.is_open()) return;

    json j;
    file >> j;

    tasks.clear();
    for (const auto& item : j){
        tasks.push_back(Task::fromJson(item));
    }

    nextId = 1;
    for (const auto& task : tasks){
    	if (task.id >= nextId) {
	    nextId = task.id + 1;
	}

    }
}

