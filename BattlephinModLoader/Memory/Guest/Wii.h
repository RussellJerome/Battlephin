#pragma once
#include <stdint.h>
#include <windows.h>
#include <iostream>

// Declare the variables (extern tells the compiler they are defined elsewhere)
extern DWORD Mem1_Size;
extern DWORD Mem2_Size;
extern DWORD Mem1RelativeLocation;
extern DWORD Mem2RelativeLocation;

class DolphinMemoryInfo;

void UpdateMemoryStartLocations();

uintptr_t GetMem1Location();
uintptr_t GetMem2Location();

// Function I wrote that gets you the actual address in dolphin / Host memory space of a game address / location
template<typename T>
T* ReadGameMemory(DWORD GameLocation)
{
    if (GameLocation >= Mem1RelativeLocation && GameLocation <= (Mem1RelativeLocation + Mem1_Size))
    {
        // Read Mem 1 Location
        DWORD adjustedOffset = GameLocation - Mem1RelativeLocation;
        DWORD64 adjustedAddress = GetMem1Location() + (DWORD64)adjustedOffset;
        return (T*)adjustedAddress;
    }
    else if (GameLocation >= Mem2RelativeLocation && GameLocation <= (Mem2RelativeLocation + Mem2_Size))
    {
        // Read Mem 2 Location
        DWORD adjustedOffset = GameLocation - Mem2RelativeLocation;
        DWORD64 adjustedAddress = GetMem2Location() + (DWORD64)adjustedOffset;
        return (T*)adjustedAddress;
    }
    else
    {
        std::cout << "No idea where we are" << std::endl;
        return nullptr;
    }
}
