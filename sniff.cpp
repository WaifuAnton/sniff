// sniff.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <shellapi.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

std::string get_ip(std::string input, std::string search);

int main()
{
    using namespace std;
    fstream file;
    
    /*int time;
    cout << "Enter the time for capturing (in seconds): ";
    cin >> time;
    time *= 1000; 

    remove("result.txt");
    fstream file;

    string host_ip;
    string mob_ip;
    vector<string> ips;

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
        mob_ip = buffer.str();
        host_ip = get_ip(host_ip, "Wireless LAN adapter Wi-Fi:");
        mob_ip = get_ip(mob_ip, "* 2:");
        ips.push_back(mob_ip);
        file.close();
    }
    else
        return -1;

    char tshark[] = "tshark.exe -f tcp -i \"Підключення через локальну мережу* 2\"";
    if (CreateProcessA(NULL, tshark, NULL, NULL, TRUE, 0, NULL, "C:\\Users\\makar\\Desktop\\Універ\\3 курс\\Мережі\\Звіти\\sniff", &si, &pi))
    {    
        Sleep(time);
        TerminateProcess(pi.hProcess, 0);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
    else
        return -1;*/

    file.open("result.txt");
    int SYN = 0;
    string str;
    while (!file.eof())
    {
        file >> str;
        if (str.find("SYN") != string::npos)
            SYN++;
    }
    file.clear();
    file.seekg(0, ios_base::beg);
    vector<string> ips;
    string mob_ip = "192.168.137.1";
    ips.push_back(mob_ip);
    string temp = mob_ip.substr(0, mob_ip.find_last_of('.'));
    int i = 0;
    while (!file.eof())
    {
        file >> str;
        bool equals = false;
        for (string ip : ips)
            if (str._Equal(ip))
            {
                equals = true;
                break;
            }
        if (str.find(temp) != string::npos && !equals)
        {
            ips.push_back(str);
            i++;
        }
    }
    file.close();
    //cout << "Host ip: " << host_ip << endl;
    for (string str : ips)
        cout << str << endl;
    cout << "Number of SYN: " << SYN << endl;
    return 0;
}

std::string get_ip(std::string input, std::string search)
{
    const int SIZE = 293;
    const int SIZE_IP = 49;
    std::string result;
    int n = input.find(search);
    result = input.substr(n, SIZE);
    n = result.find("IPv4 Address");
    result = result.substr(n, SIZE_IP);
    n = result.find(":");
    int m = result.length() - n;
    result = result.substr(n + 2, m);
    return result;
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
