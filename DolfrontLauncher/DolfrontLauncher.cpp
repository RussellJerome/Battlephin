// DolfrontLauncher.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <psapi.h>
#include <tlhelp32.h>

int EmulatorPID = 0;
void InjectDLL(std::string path)
{
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, EmulatorPID);
    if (hProc && hProc != INVALID_HANDLE_VALUE)
    {
        void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        WriteProcessMemory(hProc, loc, path.c_str(), strlen(path.c_str()) + 1, 0);
        HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
        if (hThread)
        {
            CloseHandle(hThread);
        }
    }
    if (hProc)
    {
        CloseHandle(hProc);
    }
}

int findProcByName(const char* procname)
{
	HANDLE hSnapshot;
	PROCESSENTRY32 pe;
	int pid = 0;
	BOOL hResult;
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) return 0;

	pe.dwSize = sizeof(PROCESSENTRY32);
	hResult = Process32First(hSnapshot, &pe);

	while (hResult) {
		std::wstring ws(pe.szExeFile);
		std::string szExeFile(ws.begin(), ws.end());

		if (strcmp(procname, szExeFile.c_str()) == 0) {
			pid = pe.th32ProcessID;
			break;
		}
		hResult = Process32Next(hSnapshot, &pe);
	}

	// closes an open handle (CreateToolhelp32Snapshot)
	CloseHandle(hSnapshot);
	return pid;
}

// On God this is just a copy and paste of my UML Launcher because the only function of this is to find a process and inject a loader.
int main(int argc, char* argv[])
{
    EmulatorPID = findProcByName("Dolphin.exe");
	if (EmulatorPID)
	{
		//Opps stay yappin
		std::cout << "FOUND EmulatorPID" << std::endl;

		std::string path = std::string(argv[0]);
		path = path.substr(0, path.find_last_of("/\\"));
		path = path + "\\BattlephinModLoader.dll";

		std::cout << path << std::endl;
		InjectDLL(path);

		Sleep(1000 / 60);
	}
}