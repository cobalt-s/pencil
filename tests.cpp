#include <cassert>
#include <filesystem>
#include <cstdlib>
#include <iostream>

#include "pencil.h"

using namespace std;

static void set_test_home() {
    namespace fs = std::filesystem; // use the namespace to save on lines
    fs::path tmp = fs::temp_directory_path() / "pencil_test_home";


    fs::remove_all(tmp); //get rid of all the stuff in the temp
    fs::create_directories(tmp); // create the temp

    setenv("HOME", tmp.string().c_str(), 1);
}

// all the tests.
static void test_add_single_task() {
    set_test_home();
    finishedCount = 0;
    saveFinishedCount(0);

    char prog[] = "pencil";
    char cmd[]  = "add";
    char text[] = "Buy milk";

    char* argv[] = { prog, cmd, text };
    int argc = 3;

    cmd_add(argc, argv);

    auto tasks = loadTasks();
    assert(tasks.size() == 1);
    assert(tasks[0].text == "Buy milk");
}

// 2. finishing a task should delete it.
static void test_finish_deletes_and_increments_counter() {
    set_test_home();
    finishedCount = 0;
    saveFinishedCount(0);

    {
        char prog[] = "pencil";
        char cmd[]  = "add";
        char t1[]   = "Task1";
        char t2[]   = "Task2";

        char* argv1[] = { prog, cmd, t1 };
        cmd_add(3, argv1);

        char* argv2[] = { prog, cmd, t2 };
        cmd_add(3, argv2);
    }

    auto tasks = loadTasks();
    assert(tasks.size() == 2);

    // Simulate: pencil finish 1
    {
        char prog[] = "pencil";
        char cmd[]  = "finish";
        char idx[]  = "1";

        char* argv[] = { prog, cmd, idx };
        cmd_finish(3, argv);
    }

    tasks = loadTasks();
    assert(tasks.size() == 1);
    assert(tasks[0].text == "Task2");   // first one removed

    int cnt = loadFinishedCount();
    assert(cnt == 1);
    assert(finishedCount == 1);
}

// 3. after 10 finishes the waffle party should reset the counter lol.
static void test_waffle_party_resets_after_10() {
    set_test_home();
    finishedCount = 0;
    saveFinishedCount(0);

    // Add 10 tasks: Task0..Task9
    for (int i = 0; i < 10; ++i) {
        char prog[] = "pencil";
        char cmd[]  = "add";
        string s = "Task" + to_string(i);

        vector<char> buf(s.begin(), s.end());
        buf.push_back('\0');

        char* argv[] = { prog, cmd, buf.data() };
        cmd_add(3, argv);
    }

    //finishing the task 10 times.
    for (int i = 0; i < 10; ++i) {
        char prog[] = "pencil";
        char cmd[]  = "finish";
        char idx[]  = "1";

        char* argv[] = { prog, cmd, idx };
        cmd_finish(3, argv);
    }

    int cnt = loadFinishedCount();
    assert(cnt == 0);           // reseting
    assert(finishedCount == 0);
}

int main() {
    cout << "running pencil tests.." << endl;

    test_add_single_task();
    test_finish_deletes_and_increments_counter();
    test_waffle_party_resets_after_10();

    cout << "all tests passed!" << endl;
    return 0;
}