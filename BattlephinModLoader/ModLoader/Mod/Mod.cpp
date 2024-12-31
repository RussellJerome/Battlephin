#include "Mod.h"
#include "../../Utilities/Logger.h"
#include "../../Utilities/Globals.h"

Mod* Mod::ModRef;

void Mod::DrawImGui()
{
}

//This is called by the loader (I think) to add the mod into the Global array and get throw any needed warnings
void Mod::CompleteModCreation()
{
	IsFinishedCreating = true;
	Global::GetGlobals()->AddToMods(ModRef);
	Log::Info("Core Mod Created: %s", ModName.c_str());
	if (ModLoaderVersion != MODLOADER_VERSION)
	{
		Log::Warn("Mod: %s was created with a different version of the Battlephin Mod Loader. This mod may be unstable.", ModName.c_str());
	}
}

void Mod::InitializeMod()
{

}

// This ejects the Mod cleanly to allow for hot reloading
DWORD __stdcall EjectionThread(LPVOID param)
{
	char* Modchr = (char*)param;
	auto DLLName = std::string(Modchr) + ".dll";
	auto DLLmodule = GetModuleHandleA(DLLName.c_str());
	if (DLLmodule)
	{
		FreeLibraryAndExitThread(static_cast<HMODULE>(DLLmodule), 0);
	}
	return 0;
}

// Create the ejection thread and Log it
void Mod::CleanUpMod()
{
    Log::Info("Ejecting Core Mod: %s", ModName.c_str());
	HANDLE hThread = CreateThread(nullptr, 0, EjectionThread, (LPVOID)ModName.c_str(), 0, nullptr);
	
}
