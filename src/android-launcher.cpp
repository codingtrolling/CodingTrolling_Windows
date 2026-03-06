#include <windows.h>
#include <shellapi.h>
#include <iostream>

// Colors: Cyber Dark (Background) and Cyber Cyan (Text/Accents)
#define CYBER_DARK RGB(13, 13, 13)
#define CYBER_CYAN RGB(0, 255, 255)

// Global Brush for Background
HBRUSH hBackBrush = CreateSolidBrush(CYBER_DARK);

// Function to handle the "Path Not Found" error
void RunCommand(const char* cmd) {
    // This ensures we look in the hidden deps folder
    char fullPath[MAX_PATH];
    wsprintf(fullPath, "_internal\\deps\\%s", cmd);
    
    HINSTANCE result = ShellExecute(NULL, "open", fullPath, NULL, NULL, SW_SHOWNORMAL);
    
    if ((INT_PTR)result <= 32) {
        MessageBox(NULL, "Error: Tool not found in _internal\\deps\\. Check your installer!", "System Failure", MB_ICONERROR);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CTLCOLORBTN:
        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, CYBER_CYAN);
            SetBkColor(hdcStatic, CYBER_DARK);
            return (INT_PTR)hBackBrush;
        }
        case WM_CREATE: {
            // UI Title
            CreateWindow("STATIC", "--- NEURAL LINK: ANDROID ---", WS_VISIBLE | WS_CHILD | SS_CENTER, 20, 10, 260, 20, hwnd, NULL, NULL, NULL);

            // Button 1: Mirror Screen (Requires scrcpy/adb in deps)
            CreateWindow("BUTTON", "MIRROR NEURAL FEED", WS_VISIBLE | WS_CHILD | BS_FLAT, 30, 50, 220, 40, hwnd, (HMENU)1, NULL, NULL);

            // Button 2: Install APK
            CreateWindow("BUTTON", "INJECT APK DATA", WS_VISIBLE | WS_CHILD | BS_FLAT, 30, 100, 220, 40, hwnd, (HMENU)2, NULL, NULL);

            // Button 3: ADB Shell
            CreateWindow("BUTTON", "OPEN ROOT TERMINAL", WS_VISIBLE | WS_CHILD | BS_FLAT, 30, 150, 220, 40, hwnd, (HMENU)3, NULL, NULL);
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == 1) RunCommand("scrcpy.exe");
            if (LOWORD(wParam) == 2) RunCommand("adb.exe install app.apk");
            if (LOWORD(wParam) == 3) RunCommand("cmd.exe /c _internal\\deps\\adb.exe shell & pause");
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "CyberAndroid";
    wc.hbrBackground = hBackBrush;
    wc.hCursor = LoadCursor(NULL, IDC_CROSS); // Cyber feel cursor

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(WS_EX_TOPMOST, "CyberAndroid", "ANDROID LINK", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 280, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nShowCmd);
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
