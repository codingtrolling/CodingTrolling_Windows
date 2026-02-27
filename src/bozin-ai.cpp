#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <windows.h>

using namespace std;

// Configuration path - hidden in the internal folder
const string CONFIG_FILE = ".\\_internal\\data\\bozin_config.dat";

// Global for the session
string USER_API_KEY = "";

// Function to handle API Response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Logic to save/load the key so the user isn't annoyed
void ManageAPIKey() {
    ifstream infile(CONFIG_FILE);
    if (infile.good()) {
        getline(infile, USER_API_KEY);
        infile.close();
        cout << "[SYSTEM] API Key loaded from secure storage." << endl;
    } else {
        cout << "!!! BOZIN AI: NO API KEY DETECTED !!!" << endl;
        cout << "Please enter your Gemini API Key: ";
        getline(cin, USER_API_KEY);

        // Save it for next time
        CreateDirectoryA(".\\_internal\\data\\", NULL); // Ensure directory exists
        ofstream outfile(CONFIG_FILE);
        outfile << USER_API_KEY;
        outfile.close();
        cout << "[SYSTEM] Key saved. Initializing..." << endl;
    }
}

void SendToBozin(string prompt) {
    CURL* curl;
    CURLcode res;
    string readBuffer;
    
    // Construct the dynamic URL with the USER's key
    string fullUrl = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=" + USER_API_KEY;

    curl = curl_easy_init();
    if (curl) {
        // Prepare JSON - escaped for C++
        string jsonPayload = "{\"contents\": [{\"parts\":[{\"text\":\"" + prompt + "\"}]}]}";

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, fullUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            cerr << "[CRITICAL ERROR] Failed to connect: " << curl_easy_strerror(res) << endl;
        } else {
            // Simple string parsing to extract response
            size_t start = readBuffer.find("\"text\": \"");
            if (start != string::npos) {
                start += 9;
                size_t end = readBuffer.find("\"", start);
                string responseText = readBuffer.substr(start, end - start);
                // Handle newlines in JSON
                for(size_t i = 0; i < responseText.length(); i++) {
                    if(responseText[i] == '\\' && responseText[i+1] == 'n') {
                        cout << "\n"; i++;
                    } else {
                        cout << responseText[i];
                    }
                }
                cout << endl;
            } else {
                cout << "Bozin says: 'Invalid API Key or Quota Exceeded!'" << endl;
                // Delete bad config if it failed
                remove(CONFIG_FILE.c_str());
            }
        }
        curl_easy_cleanup(curl);
    }
}

int main() {
    SetConsoleTitleA("CT-TOOL-03: BOZIN_AI_SECURE");
    system("color 0A");

    cout << "========================================" << endl;
    cout << "    BOZIN AI - USER CONTROLLED v1.1     " << endl;
    cout << "========================================" << endl;

    ManageAPIKey();

    string userInput;
    while (true) {
        cout << "\n[YOU]: ";
        getline(cin, userInput);

        if (userInput == "exit" || userInput == "quit") break;
        if (userInput == "reset-key") {
            remove(CONFIG_FILE.c_str());
            cout << "Config wiped. Restart to enter new key." << endl;
            break;
        }

        SendToBozin(userInput);
        cout << "\n----------------------------------------" << endl;
    }

    return 0;
}
