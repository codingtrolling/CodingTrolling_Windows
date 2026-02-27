#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

// Path to the hidden tools folder
string toolsPath = ".\\_internal\\bin\\"; 

void launchTool(string exeName) {
    string fullPath = toolsPath + exeName;
    ShellExecuteA(NULL, "open", fullPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

int main() {
    int choice;
    while (true) {
        system("cls");
        cout << "--- CODINGTROLLING MASTER HUB ---" << endl;
        cout << "1. Media Player\n2. PDF Reader\n3. Win1 Emulator\n0. Exit" << endl;
        cout << "> ";
        cin >> choice;

        if (choice == 0) break;
        switch(choice) {
            case 1: launchTool("media-player.exe"); break;
            case 2: launchTool("pdf-reader.exe"); break;
            case 3: launchTool("win1-launcher.exe"); break;
            default: cout << "Invalid Selection."; break;
        }
    }
    return 0;
}
