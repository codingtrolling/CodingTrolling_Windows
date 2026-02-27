#include <iostream>
#include <vector>
#include <string>
#include <libvirt/libvirt.h>
#include <windows.h>

using namespace std;

// --- VM CONFIGURATION STRUCTURE ---
struct VMSettings {
    string name;
    int memoryMB;
    int vCPUs;
    string diskPath;
};

// --- CORE VM MANAGER CLASS ---
class TrollingVMManager {
private:
    virConnectPtr conn;

public:
    TrollingVMManager() {
        // Connect to the local QEMU hypervisor
        conn = virConnectOpen("qemu:///system");
        if (conn == NULL) {
            cerr << "[ERROR] Failed to connect to hypervisor!" << endl;
        }
    }

    ~TrollingVMManager() {
        if (conn) virConnectClose(conn);
    }

    void ListVMs() {
        int numDomains = virConnectNumOfDomains(conn);
        vector<int> ids(numDomains);
        numDomains = virConnectListDomains(conn, ids.data(), numDomains);

        cout << "\n--- ACTIVE VIRTUAL MACHINES ---" << endl;
        for (int i = 0; i < numDomains; i++) {
            virDomainPtr dom = virDomainLookupByID(conn, ids[i]);
            cout << " [ID " << ids[i] << "] Name: " << virDomainGetName(dom) << endl;
            virDomainFree(dom);
        }
    }

    // Launch a VM using a simple XML definition (The "Troll" way)
    void StartVM(string vmName) {
        virDomainPtr dom = virDomainLookupByName(conn, vmName.c_str());
        if (dom && virDomainCreate(dom) == 0) {
            cout << "[SUCCESS] VM '" << vmName << "' is now running." << endl;
        } else {
            cout << "[FAILED] Could not start VM." << endl;
        }
        if (dom) virDomainFree(dom);
    }
};

int main(int argc, char* argv[]) {
    SetConsoleTitleA("CT-TOOL-06: VM_MANAGER_PRO");
    system("color 0C"); // Deep Red for the God Tool

    TrollingVMManager manager;

    int choice;
    while (true) {
        cout << "\n========================================" << endl;
        cout << "   CODINGTROLLING VM MANAGER v1.0       " << endl;
        cout << "========================================" << endl;
        cout << "1. List All Running VMs\n2. Start VM Instance\n";
        cout << "3. Create New Disk Image (qemu-img)\n0. Return to Hub" << endl;
        cout << "> ";
        cin >> choice;

        if (choice == 0) break;

        switch (choice) {
            case 1:
                manager.ListVMs();
                break;
            case 2: {
                string name;
                cout << "Enter VM Name: ";
                cin >> name;
                manager.StartVM(name);
                break;
            }
            case 3:
                // Shell out to qemu-img to create a 20GB disk
                system("qemu-img create -f qcow2 .\\_internal\\data\\new_disk.qcow2 20G");
                cout << "[SYSTEM] 20GB Disk Created." << endl;
                break;
        }
        cout << "\nPress [Enter] to continue...";
        cin.ignore(); cin.get();
    }

    return 0;
}
