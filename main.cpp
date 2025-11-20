// This is the main file for the pencil program.

#include <string>
#include <iostream>
#include "pencil.h"

using namespace std;

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

        //This is decrementing the count of tokens for making a mistake with the commands lol.
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