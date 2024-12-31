#pragma once
#include <stdlib.h>
//Swaps
#define int16_swap(x) _byteswap_ushort(x);
#define int32_swap(x) _byteswap_ulong(x);
#define int64_swap(x) _byteswap_uint64(x);