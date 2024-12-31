#pragma once
#include <string>
#include <windows.h>
#include "../../DFMLDefs.h"

//Base class layout that all mods will use
class LOADER_API Mod
{
public:
	// Mod Default Variables
	std::string ModName;
	std::string ModVersion;
	std::string ModDescription;
	std::string ModAuthors;
	std::string ModLoaderVersion;
	//ModInternals
	bool IsFinishedCreating = 0;

	//Called after each mod is injected, Looped through via gloabals
	virtual void InitializeMod();

	//Called as the mod is disabled
	virtual void CleanUpMod();

	//Call ImGui Here
	virtual void DrawImGui();

	//Called When Mod Construct Finishes
	void CompleteModCreation();

	static Mod* ModRef;
};