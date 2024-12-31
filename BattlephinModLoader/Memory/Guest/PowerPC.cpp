#include "PowerPC.h"
#include <stdlib.h>

void PowerPC::int32be::Set(int newValue)
{
	Value = _byteswap_ulong(newValue);
}

int32_t PowerPC::int32be::Get()
{
	return _byteswap_ulong(Value);
}

void PowerPC::int16be::Set(int16_t newValue)
{
	Value = _byteswap_ushort(newValue);
}

int16_t PowerPC::int16be::Get()
{
	return _byteswap_ushort(Value);
}

void PowerPC::int64be::Set(int64_t newValue)
{
	Value = _byteswap_uint64(newValue);
}

int64_t PowerPC::int64be::Get()
{
	return _byteswap_uint64(Value);
}

void PowerPC::floatbe::Set(float newValue)
{
	float retVal;
	char* floatToConvert = (char*)&newValue;
	char* returnFloat = (char*)&retVal;

	// swap the bytes into a temporary buffer
	returnFloat[0] = floatToConvert[3];
	returnFloat[1] = floatToConvert[2];
	returnFloat[2] = floatToConvert[1];
	returnFloat[3] = floatToConvert[0];

	Value = retVal;
}

float PowerPC::floatbe::Get()
{
	float retVal;
	char* floatToConvert = (char*)&Value;
	char* returnFloat = (char*)&retVal;

	// swap the bytes into a temporary buffer
	returnFloat[0] = floatToConvert[3];
	returnFloat[1] = floatToConvert[2];
	returnFloat[2] = floatToConvert[1];
	returnFloat[3] = floatToConvert[0];
	return retVal;
}

void PowerPC::uint32be::Set(int newValue)
{
	Value = _byteswap_ulong(newValue);
}

int PowerPC::uint32be::Get()
{
	return _byteswap_ulong(Value);
}

void PowerPC::uint16be::Set(int newValue)
{
	Value = _byteswap_ushort(newValue);
}

int16_t PowerPC::uint16be::Get()
{
	return _byteswap_ushort(Value);
}

void PowerPC::doublebe::Set(double newValue)
{
	Value = _byteswap_uint64(newValue);
}

double PowerPC::doublebe::Get()
{
	return _byteswap_uint64(Value);
}

void PowerPC::uint64be::Set(int64_t newValue)
{
	Value = _byteswap_uint64(newValue);
}

int64_t PowerPC::uint64be::Get()
{
	return _byteswap_uint64(Value);
}