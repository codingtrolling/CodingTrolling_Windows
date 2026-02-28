#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>

using namespace std;

// Paths to our hidden dependencies
const string QEMU_BIN = ".\\_internal\\deps\\qemu\\qemu-system-x86_64.exe";
const string IMG_TOOL = ".\\_internal\\deps\\qemu\\qemu-img.exe";
const string VM_STORAGE = ".\\_internal\\data\\vms\\";

void CreateDisk(string name, string size) {
    string cmd = IMG_TOOL + " create -f qcow2 " + VM_STORAGE + name + ".qcow2 " + size;
    cout << "[SYSTEM] Creating Virtual Disk..." << endl;
    system(cmd.c_str());
}

void LaunchVM(string diskName, int ram, bool hasIso, string isoPath = "") {
    // Building the hardware string
    // -m: RAM, -hda: Hard Drive, -vga std: Standard Graphics
    string cmd = QEMU_BIN + " -m " + to_string(ram) + "M -hda " + VM_STORAGE + diskName + ".qcow2 -vga std -rtc base=localtime";
    
    if (hasIso) {
        cmd += " -cdrom " + isoPath + " -boot d";
    }

    cout << "[SYSTEM] Powering on VM: " << diskName << "..." << endl;
    
    // Start QEMU as a separate process so the Manager stays open
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (CreateProcessA(NULL, (char*)cmd.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        cout << "[SUCCESS] VM Window Initialized." << endl;
    } else {
        cout << "[ERROR] Failed to launch QEMU engine." << endl;
    }
}

int main() {
    SetConsoleTitleA("CT-TOOL-06: VM_MANAGER_PRO");
    system("color 0C"); // Crimson Red

    // Ensure the VM storage folder exists
    CreateDirectoryA(".\\_internal\\data\\vms\\", NULL);

    while (true) {
        system("cls");
        cout << "========================================" << endl;
        cout << "    CODINGTROLLING VM MANAGER PRO       " << endl;
        cout << "========================================" << endl;
        cout << "1. Create New VM (Empty Disk)\n2. Run Windows 1 (Legacy Mode)\n";
        cout << "3. Boot ISO (Linux/Windows Installer)\n0. Exit to Hub" << endl;
        cout << "========================================" << endl;
        cout << "> ";

        int choice;
        cin >> choice;

        if (choice == 0) break;

        switch (choice) {
            case 1: {
                string name, size;
                cout << "Enter VM Name: "; cin >> name;
                cout << "Enter Disk Size (e.g. 10G, 500M): "; cin >> size;
                CreateDisk(name, size);
                break;
            }
            case 2:
                // Special preset for Win1
                LaunchVM("win1", 256, false);
                break;
            case 3: {
                string disk, iso;
                cout << "Select Disk Name: "; cin >> disk;
                cout << "Path to ISO: "; cin >> iso;
                LaunchVM(disk, 1024, true, iso);
                break;
            }
        }
        cout << "\nOperation Complete. Press Enter...";
        cin.ignore(); cin.get();
    }
    return 0;
}
