// sniff.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <shellapi.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main()
{
    using namespace std;
    const int SIZE = 293;
    const int SIZE_IP = 49;
    
    int time;
    cout << "Enter the time for capturing (in seconds): ";
    cin >> time;
    time *= 1000;

    remove("result.txt");
    fstream file;

    string host_ip;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    HANDLE h = CreateFile("result.txt",
        FILE_APPEND_DATA,
        FILE_SHARE_WRITE | FILE_SHARE_READ,
        &sa,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    STARTUPINFO si = {};
    PROCESS_INFORMATION pi = {};

    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.hStdInput = NULL;
    si.hStdError = h;
    si.hStdOutput = h;
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.wShowWindow = SW_SHOWDEFAULT;

    char ip[] = "ipconfig";

    if (CreateProcessA(NULL, ip, NULL, NULL, TRUE, 0, NULL, "C:\\Users\\makar\\Desktop\\Універ\\3 курс\\Мережі\\Звіти\\sniff", &si, &pi))
    {     
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        Sleep(1000);
        file.open("result.txt", ifstream::out | ifstream::in);       
        stringstream buffer;
        buffer << file.rdbuf();
        host_ip = buffer.str();
        int n = host_ip.find("Wireless LAN adapter Wi-Fi:");
        host_ip = host_ip.substr(n, SIZE);
        n = host_ip.find("IPv4 Address");
        host_ip = host_ip.substr(n, SIZE_IP);
        n = host_ip.find(":");
        int m = host_ip.length() - n;
        host_ip = host_ip.substr(n, m);
        cout << host_ip;      
    }

    char cmd[] = "tshark.exe -f tcp -i Wi-Fi";
    if (CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, NULL, "C:\\Users\\makar\\Desktop\\Універ\\3 курс\\Мережі\\Звіти\\sniff", &si, &pi))
    {    
        Sleep(time);
        TerminateProcess(pi.hProcess, 0);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        std::cout << "End\n";
        string a;
        file >> a;
        cout << a;
    }
    else
        return -1;

    //strcpy_s(cmd, )
    return 1;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
