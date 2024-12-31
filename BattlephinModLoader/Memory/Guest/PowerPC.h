#pragma once
#include <stdint.h>
#include "Wii.h"

//These are int and other memory value wrappers I coded to handle the byte swapping due to Wii and Xbox being PowerPC
namespace PowerPC
{
	class int16be
	{
	public:
		void Set(int16_t newValue);
		int16_t Get();
	private:
		int16_t Value;
	};

	class uint16be
	{
	public:
		void Set(int newValue);
		int16_t Get();
	private:
		unsigned __int16 Value;
	};

	class int32be
	{
	public:
		void Set(int newValue);
		int32_t Get();
	private:
		int32_t Value;
	};

	class uint32be
	{
	public:
		void Set(int newValue);
		int Get();
	private:
		unsigned int Value;
	};

	class int64be
	{
	public:
		void Set(int64_t newValue);
		int64_t Get();
	private:
		int64_t Value;
	};

	class uint64be
	{
	public:
		void Set(int64_t newValue);
		int64_t Get();
	private:
		unsigned __int64 Value;
	};

	class doublebe
	{
	public:
		void Set(double newValue);
		double Get();
	private:
		__int64 Value;
	};

	class floatbe
	{
	public:
		void Set(float newValue);
		float Get();
	private:
		float Value;
	};
};

using namespace PowerPC;

// Wii specific type that will take the place of pointers in game memory when rooting around
template<typename T>
class wiiP
{
public:
	T* Get()
	{
		T* ret = ReadGameMemory<T>((DWORD)rawPointer.Get());
		return ret;
	}

	//Mainly for debugging
	int32be GetRawPointer()
	{
		return rawPointer;
	}
private:
	int32be rawPointer;
};