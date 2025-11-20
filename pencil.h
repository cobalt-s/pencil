//
// Created by Cobalt Stamey on 11/18/25.
//

#pragma once
#include <string>
#include <vector>

struct Task {
    std::string text;
    std::string due_date;
};

extern int finishedCount;

std::string getTodoFilePath();
std::string getCounterFilePath();

std::vector<Task> loadTasks();
void saveTasks(const std::vector<Task>& tasks);

int loadFinishedCount();
void saveFinishedCount(int cnt);

void cmd_list();
void cmd_add(int argc, char* argv[]);
void cmd_finish(int argc, char* argv[]);
void print_help();