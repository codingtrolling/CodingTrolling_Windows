#include <windows.h>
#include <iostream>
#include <string>
#include <shlobj.h>

using namespace std;

// Function to check if file exists before trying to render
bool FileExists(const string& name) {
    DWORD dwAttrib = GetFileAttributesA(name.c_str());
    return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
           !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

int main(int argc, char* argv[]) {
    // Styling the console for the suite
    SetConsoleTitleA("CT-TOOL-02: PDF_READER");
    system("color 0D"); // Purple theme for PDF tool

    string pdfPath;

    // Check if the Hub passed a path directly
    if (argc > 1) {
        pdfPath = argv[1];
    } else {
        cout << "========================================" << endl;
        cout << "    CODINGTROLLING PDF RENDERER v1.0    " << endl;
        cout << "========================================" << endl;
        cout << "Enter Path to PDF file: " << endl;
        cout << "> ";
        getline(cin, pdfPath);
    }

    if (pdfPath.empty()) return 0;

    if (FileExists(pdfPath)) {
        cout << "[SYSTEM] Initializing Rendering Engine..." << endl;
        Sleep(500);
        
        // This is the core 'Shell' call. 
        // It tells Windows to open the PDF in its optimized viewer 
        // while keeping it linked to our process.
        HINSTANCE result = ShellExecuteA(
            NULL,               // Parent window
            "open",             // Action
            pdfPath.c_str(),    // File path
            NULL,               // Parameters
            NULL,               // Directory
            SW_MAXIMIZE         // Launch state
        );

        // Error handling based on Windows System Codes
        if ((INT_PTR)result <= 32) {
            cout << "[ERROR] Could not render PDF. Code: " << (INT_PTR)result << endl;
        } else {
            cout << "[SUCCESS] PDF displayed in secure container." << endl;
        }
    } else {
        cout << "[ERROR] File not found: " << pdfPath << endl;
    }

    cout << "\nPress [Enter] to return to Hub control..." << endl;
    cin.get();

    return 0;
}
