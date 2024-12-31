#pragma once
#include "ModLoader/Mod/Mod.h"

class ExampleMod : public Mod
{
public:

	//Basic Mod Info
	ExampleMod()
	{
		ModName = "ExampleMod"; // Mod Name -- Needs to be the same as the dll name
		ModVersion = "1.0.0"; // Mod Version
		ModDescription = "This is a blank Mod test."; // Mod Description
		ModAuthors = "RussellJ"; // Mod Author
		ModLoaderVersion = "1.0.0";

		// Dont Touch The Internal Stuff
		ModRef = this;
		CompleteModCreation();
	}

	//Called When Internal Mod Setup is finished
	virtual void InitializeMod() override;

	//Called when Internal Mod is finishing up before ejection
	virtual void CleanUpMod() override;

	//Call ImGui Here (CALLED EVERY FRAME ON DX HOOK)
	virtual void DrawImGui() override;
};