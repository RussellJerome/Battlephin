#include "Wii.h"
#include <windows.h>
#include <iostream>
#include "../Host/VariableGrabber.h"

// Just basic wii stuff like mem size and relative Locations
DWORD Mem1_Size = 0x3FFFFF0;
DWORD Mem2_Size = 0x7FFFFF0;

DWORD Mem1RelativeLocation = 0x80000000;
DWORD Mem2RelativeLocation = 0x90000000;

//Wrapper for a random pointer I backtracked from dolphin
class DolphinMemoryInfo
{
public:
    uintptr_t Mem1;
    uintptr_t Mem2;

public:
    static DolphinMemoryInfo** MemoryInfo;
};

DolphinMemoryInfo* GetMemoryInfo()
{
    return *DolphinMemoryInfo::MemoryInfo;
}

DolphinMemoryInfo** DolphinMemoryInfo::MemoryInfo;

// When called rips static pointer that has 2 pointers to both Mem locations in dolphin
void UpdateMemoryStartLocations()
{
    //I actually dont know what this pattern is to. I used cheat engine to find a normal offset then genned a pattern with that in IDA so this should work across dolphin versions
    auto MemSetupPattern = new Memory::Host::VariableGrabber("48 89 0D ? ? ? ? 48 8D 48 ? 89 15 ? ? ? ? 8B 50", 3, 7);
    auto MemArrayPtr = MemSetupPattern->ripVariable();

    DolphinMemoryInfo::MemoryInfo = (DolphinMemoryInfo**)MemArrayPtr;
}

uintptr_t GetMem1Location()
{
    return GetMemoryInfo()->Mem1;
}

uintptr_t GetMem2Location()
{
    return GetMemoryInfo()->Mem2;
}
