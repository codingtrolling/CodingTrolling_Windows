#include <windows.h>
#include <shellapi.h>

// Function to launch your tools from the hidden folder
void LaunchTool(const char* toolName) {
    char path[MAX_PATH];
    // This points to the hidden _internal/bin folder we set up
    wsprintf(path, "_internal\\bin\\%s", toolName);
    ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
}

// Window Procedure (Handles button clicks)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // Create 7 Buttons for your 7 Tools
            CreateWindow("BUTTON", "1. Media Player", WS_VISIBLE | WS_CHILD, 20, 20, 150, 30, hwnd, (HMENU)1, NULL, NULL);
            CreateWindow("BUTTON", "2. PDF Reader", WS_VISIBLE | WS_CHILD, 20, 60, 150, 30, hwnd, (HMENU)2, NULL, NULL);
            CreateWindow("BUTTON", "3. Bozin AI", WS_VISIBLE | WS_CHILD, 20, 100, 150, 30, hwnd, (HMENU)3, NULL, NULL);
            CreateWindow("BUTTON", "4. Android Launch", WS_VISIBLE | WS_CHILD, 20, 140, 150, 30, hwnd, (HMENU)4, NULL, NULL);
            CreateWindow("BUTTON", "5. Panny Paint", WS_VISIBLE | WS_CHILD, 20, 180, 150, 30, hwnd, (HMENU)5, NULL, NULL);
            CreateWindow("BUTTON", "6. VM Manager", WS_VISIBLE | WS_CHILD, 20, 220, 150, 30, hwnd, (HMENU)6, NULL, NULL);
            CreateWindow("BUTTON", "7. Windows 1", WS_VISIBLE | WS_CHILD, 20, 260, 150, 30, hwnd, (HMENU)7, NULL, NULL);
            break;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case 1: LaunchTool("media-player.exe"); break;
                case 2: LaunchTool("pdf-reader.exe"); break;
                case 3: LaunchTool("bozin-ai.exe"); break;
                case 4: LaunchTool("android-launcher.exe"); break;
                case 5: LaunchTool("panny-paint.exe"); break;
                case 6: LaunchTool("vm-manager.exe"); break;
                case 7: LaunchTool("vm-manager.exe"); break; // Launching VM for Win1
            }
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    const char CLASS_NAME[] = "CodingTrollingHub";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "CodingTrolling Suite v1.0", WS_OVERLAPPEDWINDOW, 
        CW_USEDEFAULT, CW_USEDEFAULT, 220, 360, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nShowCmd);
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
