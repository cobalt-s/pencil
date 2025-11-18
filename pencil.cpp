/*
This is Pencil, this is my own personal to-do list.

I aim for this in the future to recognize keywords in the text and
auto put things like time and date.

Its just very simple right now.

Author: Cobalt Stamey
Email: stameycobalt@gmail.com
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define TODO_FILE ".pencil" // this is the default file name for the todo storage.

using namespace std;

struct Task
{
    bool done;
    string text;
};

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
        out << (t.done ? '1' : '0') << "|" << t.text << endl;
    }
}

// this gets the counter file path so its saved between runs
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

    cout << "These are your tasks: " << endl;
    cout << "Complete them for a waffle token... :)" << endl;

    int taskNumber = 1;
    for (const auto &t : tasks)
    {
        cout << taskNumber << ". " << "[" << (t.done ? 'x' : ' ') << "] " << t.text << endl;
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
    tasks.push_back({false, text});
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
        cout << "Usage: pencil done <task-number>" << endl;
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

void print_help()
{
    cout << "Welcome to pencil, the ultimate CLI todolist!" << endl;
    cout << "====================================" << endl;
    cout << "Usage:" << endl;
    cout << " pencil list" << endl;
    cout << " pencil add <task text>" << endl;
    cout << " pencil finish <task-number>" << endl;
}

int main(int argc, char *argv[])
{
    // load persisted counter at startup
    finishedCount = loadFinishedCount();

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

        if (finishedCount == 0)
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
            saveFinishedCount(finishedCount);
        }
        return 1;
    }

    return 0;
}