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
#include <limits>
#include <map>

std::string get_ip(std::string input, std::string search);
std::fstream& GotoLine(std::fstream& file, unsigned int num);

int main()
{
    using namespace std;
    
    int time;
    cout << "Enter the time for capturing (in seconds): ";
    cin >> time;
    time *= 1000; 

    remove("result.txt");
    fstream file;

    string host_ip;
    string mob_ip;
    vector<string> ips;
    vector<string> ports;

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
    si.dwFlags = STARTF_USESTDHANDLES;
    si.wShowWindow = SW_SHOWDEFAULT;

    char ip[] = "ipconfig";

    if (CreateProcessA(NULL, ip, NULL, NULL, TRUE, 0, NULL, "C:\\Users\\makar\\Desktop\\Універ\\3 курс\\Мережі\\Звіти\\Практ1\\sniff", &si, &pi))
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
    {
        DWORD rc = GetLastError();
        cout << rc;
        return -1;
    }

    char tshark[] = "tshark.exe -i \"Підключення через локальну мережу* 2\"";
    if (CreateProcessA(NULL, tshark, NULL, NULL, TRUE, 0, NULL, "C:\\Users\\makar\\Desktop\\Універ\\3 курс\\Мережі\\Звіти\\Практ1\\sniff", &si, &pi))
    {    
        Sleep(time + 200);
        TerminateProcess(pi.hProcess, 0);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
    else
        return -1;
  
    file.open("result.txt");
    string str;
    string temp = mob_ip.substr(0, mob_ip.find_last_of('.'));
    while (!file.eof())
    {
        file >> str;
        bool equals = false;
        for (string ip : ips)
            if (str._Equal(ip) || str.data()[str.size() - 1] == '?')
            {
                equals = true;
                break;
            }
        if (str.find(temp) != string::npos && !equals)
            ips.push_back(str);
    }
    file.close();

    vector<string> tmp;
    file.open("result.txt");
    GotoLine(file, 55);
    while (getline(file, str))
        tmp.push_back(str.substr(8, str.size() - 8));

    vector<string> words;
    vector<string> sites;
    map<string, int> syns;
    map<string, string> flood;
    for (int i = 1; i < ips.size(); i++)
        syns[ips.at(i)] = 0;
    int syn = 0, syn_all = 0;
    for (int i = 0; i < tmp.size() - 1; i++)
    {
        string word = "";
        for (int j = 0; j < tmp[i].size(); j++)
        {
            if (j == tmp[i].size() - 1)
            {
                word += tmp[i][j];
                words.push_back(word);
                word = "";
            }
            else if (tmp[i][j] == ' ')
            {               
                words.push_back(word);
                word = "";
            }
            else
                word += tmp[i][j];
        }
        if (!words.size())
            continue;            
        if (words.size() > 9)
        {        
            ports.push_back(words.at(6));
            ports.push_back(words.at(8));
            if (words.at(4)._Equal("DNS") && words.at(8)._Equal("response"))
                sites.push_back(words.at(11));
            if (!words.at(9)._Equal("[SYN]"))
            {
                words.clear();
                continue;
            }
            syns[words.at(1)]++;
            syn_all++;
            flood[words.at(1)] = words.at(3);
        }
        words.clear();
    }

    vector<string> ports_total;
    for (int i = 0; i < ports.size(); i++)
        if (isdigit((unsigned char)ports.at(i).data()[0]))
            ports_total.push_back(ports.at(i));
    file.close();


    cout << "Host ip: " << ips.at(0) << endl << endl;
    cout << "Ports:\n";
    for (string port : ports_total)
        cout << port << " ";
    cout << endl << endl;
    for (int i = 1; i < ips.size(); i++)
        cout << ips[i] << "; ";
    cout << endl << endl;
    for (string site : sites)
        cout << site << endl;
    cout << endl;
    cout << "Number of SYN: " << syn_all << endl;
    for (auto s = syns.cbegin(); s != syns.cend(); s++)
        cout << s->first << ": " << s->second << endl;
    for (auto f = flood.cbegin(); f != flood.cend(); f++)
        cout << f->first << " --> " << f->second << endl;
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

std::fstream& GotoLine(std::fstream& file, unsigned int num) {
    file.seekg(std::ios::beg);
    for (int i = 0; i < num - 1; ++i)
        file.ignore(256, '\n');
    return file;
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
