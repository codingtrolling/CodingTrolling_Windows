#include <iostream>
#include <string>
#include <windows.h>
#include <vector>

using namespace std;

// Path to the hidden ADB engine
const string ADB_PATH = ".\\_internal\\bin\\adb\\adb.exe";
const string SCRCPY_PATH = ".\\_internal\\bin\\scrcpy\\scrcpy.exe";

// Helper to run a command and hide the black CMD window
void ExecuteHidden(string cmd) {
    string fullCmd = "/C " + cmd;
    ShellExecuteA(NULL, "open", "cmd.exe", fullCmd.c_str(), NULL, SW_HIDE);
}

// Function to run ADB and show output in our suite
void RunADB(string args) {
    string command = ADB_PATH + " " + args;
    system(command.c_str());
}

int main() {
    SetConsoleTitleA("CT-TOOL-04: ANDROID_LAUNCHER");
    system("color 0E"); // Yellow theme

    int choice;
    while (true) {
        cout << "========================================" << endl;
        cout << "    ANDROID LAUNCHER & BRIDGE v1.0      " << endl;
        cout << "========================================" << endl;
        cout << "1. List Connected Devices\n2. Mirror Screen (SCRCPY)\n3. Take Screenshot\n";
        cout << "4. Reboot Device\n5. Install APK\n0. Back to Hub" << endl;
        cout << "========================================" << endl;
        cout << "> ";
        cin >> choice;

        if (choice == 0) break;

        switch (choice) {
            case 1:
                cout << "[SYSTEM] Scanning for devices..." << endl;
                RunADB("devices");
                break;
            case 2:
                cout << "[SYSTEM] Initializing Screen Mirror..." << endl;
                // Launching scrcpy as a separate process so it doesn't block the launcher
                ShellExecuteA(NULL, "open", SCRCPY_PATH.c_str(), NULL, NULL, SW_SHOWNORMAL);
                break;
            case 3: {
                string filename;
                cout << "Enter filename (e.g., shot.png): ";
                cin >> filename;
                RunADB("shell screencap -p /sdcard/" + filename);
                RunADB("pull /sdcard/" + filename + " .");
                cout << "[SUCCESS] Saved to project folder." << endl;
                break;
            }
            case 4:
                cout << "[SYSTEM] Sending Reboot Signal..." << endl;
                RunADB("reboot");
                break;
            case 5: {
                string apk;
                cout << "Drag and Drop APK here: ";
                cin >> apk;
                RunADB("install " + apk);
                break;
            }
            default:
                cout << "Invalid Command." << endl;
        }
        cout << "\nPress [Enter] to continue...";
        cin.ignore(); cin.get();
        system("cls");
    }
    return 0;
}
