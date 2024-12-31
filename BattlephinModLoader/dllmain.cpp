// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include "ModLoader/ModLoader.h"

//Mod loader is born and dies in here
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ModLoader::SetupModLoader();
        break;
    case DLL_PROCESS_DETACH:
        ModLoader::CleanUpModLoader();
        break;
    }
    return TRUE;
}