#pragma once
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include "../DFMLDefs.h"

//Similar to my UML globals, just stuff to be called anywhere
class LOADER_API Global {
public:
	std::vector<class Mod*> CoreMods;
	bool bExit = false;
	static Global* GetGlobals();

	void AddToMods(Mod* mod)
	{
		CoreMods.push_back(mod);
	}

private:
	static Global* glb;
};