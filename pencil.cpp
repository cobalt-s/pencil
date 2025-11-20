/*
This is Pencil, this is my own personal to-do list.

I aim for this in the future to recognize keywords in the text and
auto put things like time and date.

To update this:
cmake --build cmake-build-debug --target pencil_install


Its just very simple right now.

Author: Cobalt Stamey
Email: stameycobalt@gmail.com
*/

#include "pencil.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define TODO_FILE ".pencil" // this is the default file name for the todo storage.

using namespace std;

// this counts every time you finish a task and after 10 you get a waffle party lol
int finishedCount = 0;

/**
 * This function finds the home directory and gets the todo file path.
 *
 * @return the Todo file path from the home directory.
 */
string getTodoFilePath()
{
    const char *home = getenv("HOME"); // gets the home dir.

    if (!home)
    {
        return {TODO_FILE};
    }

    return string(home) + "/" + TODO_FILE; // returns the home dir.
}

/**
 * Loads the tasks from the ToDo
 *
 * @return a list of the tasks in the todo.
 */
vector<Task> loadTasks()
{
    vector<Task> tasks;

    ifstream in(getTodoFilePath());
    if (!in)
    {
        return tasks;
    }

    string line;
    while (getline(in, line))
    {
        if (line.empty()) {
            continue;
        }

        Task t;
        t.text = line;
        tasks.push_back(t);
    }

    return tasks;
}

/**
 * Saves the tasks to the todo file path.
 * @param tasks a vector of the tasks that were loaded.
 */
void saveTasks(const vector<Task> &tasks)
{
    ofstream out(getTodoFilePath());
    if (!out)
    {
        cerr << "Error: cannot write to todo file." << endl;
    }

    for (const auto &t : tasks)
    {
        out << t.text << "|" << t.due_date <<  endl;
    }
}

// this gets the counter file path so it's saved between runs
/**
 * This gets the file path for the file that saves
 * the count of tokens.
 *
 * @return the counter of tokens save file.
 */
string getCounterFilePath()
{
    const char *home = getenv("HOME");
    if (!home)
    {
        return string(".pencil_count");
    }
    return string(home) + "/.pencil_count";
}

/**
 * Loads the amount of tasks completed.
 *
 * @return the count of tasks completed.
 */
int loadFinishedCount()
{
    ifstream in(getCounterFilePath());
    if (!in)
        return 0;
    int cnt = 0;
    in >> cnt;
    if (!in)
        return 0;
    return cnt;
}

/**
 * Saves the amount of tasks completed to the counter file path.
 *
 * @param cnt the amount of tasks completed.
 */
void saveFinishedCount(int cnt)
{
    ofstream out(getCounterFilePath());
    if (!out)
    {
        cerr << "Warning: cannot write to counter file." << endl;
        return;
    }
    out << cnt << endl;
}

/**
 * Displays the list of tasks.
 */
void cmd_list()
{
    auto tasks = loadTasks();
    if (tasks.empty())
    {
        cout << "No tasks yet" << endl;
        return;
    }

    cout << "Tasks for Today: " << endl;

    int taskNumber = 1;
    for (const auto &t : tasks)
    {
        cout << taskNumber << ". "  << t.text << " " << t.due_date << endl;
        ++taskNumber;
    }
}

/**
 * The add command, adds a task to the list of tasks.
 *
 * @param argc The amount of arguments
 * @param argv The data of the arguments.
 */
void cmd_add(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Usage: pencil add <task text>" << endl;
        return;
    }

    string text = argv[2];

    for (int i = 3; i < argc; ++i)
    {
        text += " ";
        text += argv[i];
    }

    auto tasks = loadTasks();
    tasks.push_back({text});
    saveTasks(tasks);

    cout << "Added your task: " << text << endl;
}

/**
 * Finishes a task in the list of tasks.
 *
 * @param argc
 * @param argv
 */
void cmd_finish(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Usage: pencil finish <task-number>" << endl;
        return;
    }

    int num = stoi(argv[2]);

    if (num <= 0)
    {
        cout << "Invalid task number" << endl;
        return;
    }

    auto tasks = loadTasks();

    if (num > static_cast<int>(tasks.size()))
    {
        cout << "Task " << num << " doesn't exist." << endl;
        return;
    }

    string removedText = tasks[num - 1].text;
    tasks.erase(tasks.begin() + (num - 1));
    saveTasks(tasks);

    ++finishedCount;
    saveFinishedCount(finishedCount);

    cout << "Deleted task " << num << ". " << removedText << endl;

    if (finishedCount < 10)
    {
        cout << 10 - finishedCount << " tasks left to get something fun..." << endl;
    }
    else
    {
        cout << "WAFFLE PARTY WOOHOO! YOU ROCK!" << endl;
        finishedCount = 0;
        saveFinishedCount(finishedCount);
    }
}

/**
 * Prints the help screen.
 */
void print_help()
{
    //@todo add a feature where it says hi to the user
    cout << "Welcome to pencil, the ultimate CLI todolist!" << endl;
    cout << "Update 0.001" << endl;
    cout << "====================================" << endl;
    cout << "Usage:" << endl;
    cout << " pencil list" << endl;
    cout << " pencil add <task text>" << endl;
    cout << " pencil finish <task-number>" << endl;
}

