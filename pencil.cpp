/*
This is Pencil, this is my own personal to-do list.

I aim for this in the future to recognize keywords in the text and
auto put things like time and date.

Its just very simple right now.

Author: Cobalt Stamey
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

#define TODO_FILE ".pencil" // this is the default file name for the todo storage.

using namespace std;

struct Task
{
    bool done;
    string text;
};

// this counts every time you finish a task and after 10 you get a waffle party lol
int finishedCount = 0;
string getTodoFilePath()
{
    const char *home = getenv("HOME"); // gets the home dir.

    if (!home)
    {
        return string(TODO_FILE);
    }

    return string(home) + "/" + TODO_FILE; // returns the home dir.
}

vector<Task> loadTasks()
{
    vector<Task> tasks;

    ifstream in(getTodoFilePath());
    if (!in)
    {
        // no tasks or file.
        return tasks;
    }

    // otherwise we move on to the tasks in the file.
    string line;

    while (getline(in, line))
    {
        // check line length
        if (line.size() < 3)
        {
            continue;
        }
        bool done = (line[0] == '1'); // task is done.
        string text = line.substr(2);
        tasks.push_back({done, text}); // adds the task is done level to vector.
    }
    return tasks;
}

void saveTasks(const vector<Task> &tasks)
{
    ofstream out(getTodoFilePath());
    if (!out)
    {
        cerr << "Error: cannot write to todo file." << endl;
    }

    for (const auto &t : tasks)
    {
        out << (t.done ? '1' : '0') << "|" << t.text << endl;
    }
}

void cmd_list()
{
    auto tasks = loadTasks();
    if (tasks.empty())
    {
        cout << "No tasks yet" << endl;
        return;
    }

    cout << "These are your tasks: " << endl;
    cout << "Complete them for a waffle token... :)" << endl;

    int taskNumber = 1;
    for (const auto &t : tasks)
    {
        cout << taskNumber << ". " << "[" << (t.done ? 'x' : ' ') << "] " << t.text << endl;
        ++taskNumber;
    }
}

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
    tasks.push_back({false, text});
    saveTasks(tasks);

    cout << "Added your task: " << text << endl;
}

void cmd_finish(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Usage: pencil done <task-number>" << endl;
        return;
    }

    int num = atoi(argv[2]);

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

    cout << "Deleted task " << num << ". " << removedText << endl;

    if (finishedCount < 10)
    {
        cout << 10 - finishedCount << " tasks left to get something fun..." << endl;
    }
    else
    {
        // WAFFLE PARTY
        cout << "WAFFLE PARTY WOOHOO! YOU ROCK!" << endl;
        finishedCount = 0; // reseting the count lol
    }
}

void print_help()
{
    cout << "Welcome to pencil, my cli todo list!" << endl;
    cout << "====================================" << endl;
    cout << "Usage:" << endl;
    cout << " pencil list" << endl;
    cout << " pencil add <task text>" << endl;
    cout << " pencil finish <task-number>" << endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        print_help();
        return 0;
    }

    string cmd = argv[1];

    if (cmd == "list")
    {
        cmd_list();
    }
    else if (cmd == "add")
    {
        cmd_add(argc, argv);
    }
    else if (cmd == "finish")
    {
        cmd_finish(argc, argv);
    }
    else if (cmd == "help")
    {
        print_help();
    }
    else
    {
        cout << "UNKNOWN COMMAND FIX IT: " << cmd << endl;
        print_help();

        if (finishedCount = 0)
        {
            cout << "you lucked out... no waffle party tokens to lose next time.....WATCH OUT!" << endl;
        }
        else
        {
            cout << "You have lost one waffle party token. " << endl;
            cout << "Current: " << finishedCount << " tokens" << endl;
            cout << "New: " << finishedCount - 1 << " tokens" << endl;
            cout << "Don't mess up next time." << endl;

            finishedCount -= 1;
        }
        return 1;
    }

    return 0;
}