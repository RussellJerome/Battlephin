#include "VariableGrabber.h"
#include "Pattern.h"

//I stole this from myself, idk if that makes me a pirate or genius but either way I wanna cry
namespace Memory
{
	namespace Host
	{
		void VariableGrabber::addAlternativeAob(const char* aob, int firstOpCodes, int totalByteInstruction)
		{
			VariableInfo info;
			info.aob = aob;
			info.FirstOpCodes = firstOpCodes;
			info.TotalByteInstruction;
			variableLocations.push_back(info);
		}

		void VariableGrabber::addAlternativeAob(const char* aob, int offset)
		{
			VariableInfo info;
			info.aob = aob;
			info.Offset = offset;
			variableLocations.push_back(info);
		}

		DWORD64 VariableGrabber::ripVariable()
		{
			if (variableLocations.size() > 0)
			{
				if (VariableType == 0)
				{
					for (size_t i = 0; i < variableLocations.size(); i++)
					{
						const auto& varlocation = variableLocations[i];
						auto Pattern = Memory::Host::Find(varlocation.aob);
						if (Pattern)
						{
							auto varaddress = *reinterpret_cast<uint32_t*>(Pattern + varlocation.FirstOpCodes);
							auto var = (DWORD64)(Pattern + varlocation.TotalByteInstruction + varaddress);
							if (var != 0)
							{
								return (DWORD64)var;
							}
						}
					}
				}
				else if (VariableType == 1)
				{
					for (size_t i = 0; i < variableLocations.size(); i++)
					{
						const auto& varlocation = variableLocations[i];
						auto Pattern = Memory::Host::Find(varlocation.aob);
						if (Pattern)
						{
							auto var = (DWORD64)(Pattern + varlocation.Offset);
							if (var != 0)
							{
								return (DWORD64)var;
							}
						}
					}
				}
			}
			return 0;
		}
	};
};