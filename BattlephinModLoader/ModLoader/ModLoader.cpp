#include "ModLoader.h"
#include <iostream>
#include <process.h>
#include <filesystem>
#include "../Utilities/Globals.h"
#include "../Utilities/Logger.h"
#include "../GUI/UI.h"
#include "Mod/Mod.h"

namespace fs = std::filesystem;

// Basic dll injection function
void InjectDLL(std::string path)
{
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, _getpid());
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

//Basic helper function to get the path of a module on disk
std::string GetModuleFilePath(HMODULE hModule)
{
    std::string ModuleName = "";
    char szFileName[MAX_PATH] = { 0 };

    if (GetModuleFileNameA(hModule, szFileName, MAX_PATH))
        ModuleName = szFileName;

    return ModuleName;
}

// Loops every dll in the Mods folder and injects them 1 by 1
void LoadMods()
{
    Log::Info("Loading Runtime Mods");
    auto Module = GetModuleHandleA("BattlephinModLoader.dll");
    std::string path = GetModuleFilePath(Module);
    path = path.substr(0, path.find_last_of("/\\"));
    path = path + "\\Mods\\";
    if (!std::filesystem::exists(path))
    {
        std::filesystem::create_directory(path);
    }
    for (const auto& entry : fs::directory_iterator(path))
    {
        if (entry.path().extension().string() == ".dll")
        {
            std::wstring path = entry.path();
            std::string str(path.begin(), path.end());
            //Mmm yes inject me harder
            InjectDLL(str);
        }
    }
}

// Loops every mod and calls their respective clean up functions
void UnloadMods()
{
    Log::Info("Unloading Mods");
    for (size_t i = 0; i < Global::GetGlobals()->CoreMods.size(); i++)
    {
        auto Mod = Global::GetGlobals()->CoreMods[i];
        Mod->CleanUpMod();
    }
}

// I dont know why I needed this but I guess I did
void SetupConsole()
{
    // Free any existing console
    FreeConsole();

    // Allocate a new console
    if (AllocConsole())
    {
        FILE* file;
        freopen_s(&file, "CONOUT$", "w", stdout);
        freopen_s(&file, "CONIN$", "r", stdin);
        freopen_s(&file, "CONOUT$", "w", stderr);
        Log::Info("Setting Up Mod Loader");
        std::cout.clear();
    }
    else
    {
        MessageBoxA(nullptr, "Failed to allocate console", "Error", MB_OK | MB_ICONERROR);
    }
}

// Function to eject the Loader DLL
void EjectSelf()
{
    auto lib = GetModuleHandleA("BattlephinModLoader.dll");
    if (lib)
    {
        FreeLibraryAndExitThread(static_cast<HMODULE>(lib), 0);
    }
}

// Function that will close the client when the modloader is exiting
void CloseConsole()
{
    // Hide the console window
    HWND hWnd = GetConsoleWindow();
    if (hWnd)
    {
        ShowWindow(hWnd, SW_HIDE);
    }

    // Free the console
    FreeConsole();
}

// Backend Logic thread for the Loader, will setup Mods and also acts as the render thread
DWORD __stdcall LoaderThread(LPVOID)
{
    LoadMods();
    Log::Info("Creating UI");
    UI::Start();
    UI::Render();
    while (true) {
        if (Global::GetGlobals()->bExit) {

            Log::Info("Exiting Mod Loader");
            //Do Once
            Global::GetGlobals()->bExit = false;
            UnloadMods();
            CloseConsole();
            Sleep(1000);
            EjectSelf();
            break;

        }
        Sleep(100); // Add a small delay
    }
    return 0;
}

// Setup console and creates logic thread
void ModLoader::SetupModLoader()
{
    SetupConsole();
    HANDLE hThread = CreateThread(nullptr, 0, LoaderThread, nullptr, 0, nullptr);
}

// Dies
void ModLoader::CleanUpModLoader()
{
    // Me after writing a whole Runtime Mod program knowing fully that this will never be used and that it provides nothing of significance to anyone. But its okay because I like doing fun projects with
    // *INHALES*
    // no use or end game. = )
    // https://www.youtube.com/watch?v=W_cPu5VuAu8
    Log::Info("Goodbye World");
}
