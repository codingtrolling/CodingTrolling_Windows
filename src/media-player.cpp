#include <windows.h>
#include <dshow.h>
#include <iostream>
#include <string>

#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "ole32.lib")

using namespace std;

void PlayVideo(string filename) {
    IGraphBuilder *pGraph = NULL;
    IMediaControl *pControl = NULL;
    IMediaEvent   *pEvent = NULL;
    IMediaWindow  *pWindow = NULL;

    // 1. Initialize the COM library
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        cout << "ERROR: Could not initialize COM." << endl;
        return;
    }

    // 2. Create the Filter Graph Manager
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
                          IID_IGraphBuilder, (void **)&pGraph);
    
    if (FAILED(hr)) {
        cout << "ERROR: Could not create Filter Graph Manager." << endl;
        return;
    }

    // 3. Query for control and event interfaces
    pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
    pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
    pGraph->QueryInterface(IID_IMediaWindow, (void **)&pWindow);

    // 4. Build the graph for the specific file
    // Convert string to WideString for Windows API
    std::wstring widestr = std::wstring(filename.begin(), filename.end());
    hr = pGraph->RenderFile(widestr.c_str(), NULL);

    if (SUCCEEDED(hr)) {
        // 5. Setup the Video Window
        pWindow->put_Caption(L"CodingTrolling Media Engine");
        pWindow->put_WindowStyle(WS_OVERLAPPEDWINDOW);
        
        // 6. Start playback
        hr = pControl->Run();
        if (SUCCEEDED(hr)) {
            cout << "Playing: " << filename << endl;
            cout << "Press [Enter] to stop playback..." << endl;
            
            long evCode;
            // Wait for completion or user interrupt
            pEvent->WaitForCompletion(INFINITE, &evCode);
        }
    } else {
        cout << "ERROR: File format not supported or file not found." << endl;
    }

    // 7. Clean up resources
    pControl->Release();
    pEvent->Release();
    pWindow->Release();
    pGraph->Release();
    CoUninitialize();
}

int main(int argc, char* argv[]) {
    // Set Console UI
    SetConsoleTitleA("CT-TOOL-01: MEDIA_PLAYER");
    system("color 0B");

    string targetFile;

    if (argc > 1) {
        // If the Hub passes a file path as an argument
        targetFile = argv[1];
    } else {
        cout << "========================================" << endl;
        cout << "   CODINGTROLLING MEDIA PLAYER v1.0     " << endl;
        cout << "========================================" << endl;
        cout << "Enter Media Path (Direct Link or Local): " << endl;
        cout << "> ";
        getline(cin, targetFile);
    }

    if (targetFile.empty()) return 0;

    PlayVideo(targetFile);

    return 0;
}
