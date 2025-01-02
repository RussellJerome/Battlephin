#pragma once
#include "../Memory/Guest/PowerPC.h"
#include "../Memory/Helper.h"
#include "../Memory/Guest/Wii.h"
#include <windows.h>

//Hand reversed classes from r911 because of course the core classes for the engine just had to fucking change making my class dump nothing more then a reference

class CBaseObject;

struct MGRRecord
{
    wiiP<char> FileName;
    wiiP<char> dictKey;
    wiiP<CBaseObject> ManagerRefObject;
    int32be WeirdObjectTypeNameUk1;

    //Assumed
    int8_t reloadIfFileChanges;
    int8_t assertIfFail;
    int8_t isReseting;
    int8_t hasDoneIt;

    //EngineAdditions I guess??????
    int8_t unk2;
    int8_t unk3;
    int8_t unk4;
    int8_t unk5;
};

template<typename T>
class IContainerT
{
    //Probably not right
    int32be IContainerVTable;
};

//Same from r7
template<typename T>
struct listState_s
{
    int32be maxSize;
    int32be count;
    wiiP<T> Itemsptr;
    int32be flags;
};

// The variables are the same as r7 but I did write my own functions to help the class actually be used in our case
template<typename T>
class CList : IContainerT<T>
{
public:
    int GetCount()
    {
        return list.count.Get();
    };

    int GetMaxSize()
    {
        return list.maxSize.Get();
    };

    bool IsValidIndex(int Index)
    {
        return Index <= GetCount(); // Fixed off-by-one error
    }

    template<typename T>
    T* GetIndex(int Index)
    {
        if (IsValidIndex(Index))
        {
            int indexOffset = sizeof(T) * Index;
            DWORD64 BaseItemAddy = (DWORD64)list.Itemsptr.Get();
            DWORD64 ItemAddress = BaseItemAddy + (DWORD64)indexOffset;
            T* ret = (T*)ItemAddress;
            return ret;
        }
        return nullptr;
    }
    listState_s<T> list;
    int32be heap;
};

// Basic outline of the first 3 vtable functions in CBaseObject. Mainly done so I could have a way to get a Game Props class name and ID
struct CBaseObject_Vtable
{
    char UnknownPadding[0x8];
    int32be DestructorPtr;
    wiiP<char> GetClassIDPtr;
    wiiP<char> GetObjectTypePtr;

public:

    // This shit is wacky, literally reads the asm bytes from the vtable function then just does what the asm would have done but in windows to get us our Object type
    int32be GetObjectType()
    {
        auto ObjectTypeAsm = GetObjectTypePtr.Get();
        uint16_t high = (ObjectTypeAsm[2] << 8) | ObjectTypeAsm[3];
        uint16_t low = (ObjectTypeAsm[6] << 8) | ObjectTypeAsm[7];
        uint32_t result = (static_cast<uint32_t>(high) << 16) | low;
        int32be ret;
        ret.Set(result);
        return ret;
    }

    // Like the above function this reads the function asm bytes at the ClassID vtable function, then rips out the string variable location and throws it too us. Neat
    const char* GetClassID()
    {
        auto ClassIDAsm = GetClassIDPtr.Get();
        uint16_t lis_imm = (static_cast<uint8_t>(ClassIDAsm[2]) << 8) | static_cast<uint8_t>(ClassIDAsm[3]); // lis
        uint16_t addi_imm = (static_cast<uint8_t>(ClassIDAsm[6]) << 8) | static_cast<uint8_t>(ClassIDAsm[7]); // addi
        if (addi_imm & 0x8000) { 
            lis_imm -= 1;
        }
        uint32_t address = (lis_imm * 0x10000) + addi_imm;
        auto ret = ReadGameMemory<const char>((DWORD)address);
        return ret;
    }
};

// The base of litterally every major game object.... and ofc it was changed from r7
class CBaseObject
{
public:
    // I think this started the black death
    int32be Unknown;

    // THEY TOOK MY SAVE TEMPLATES FROM ME THOSE MONSTERS
    int32be unknownMayBem_saveTemplate;

    // IDK
    int32be ActualUnknownData;

    // Why the hell is the VTable pointer 12 bytes from the base? Like I have no idea if that is a wii thing or if FRD just decided to say fuck it we first fighting God.
    wiiP<CBaseObject_Vtable> ObjectVTablePtr;

public:
    int32be GetObjectType()
    {
        return ObjectVTablePtr.Get()->GetObjectType();
    }

    const char* GetClassID()
    {
        return ObjectVTablePtr.Get()->GetClassID();
    }
};

template<typename T>
class CSingletonT : CBaseObject
{
};

// Stayed basically the same
class CMgrMgr : CSingletonT<CMgrMgr>
{
public:
    CList<MGRRecord> m_mgrs;
    int32be m_strPool; // CStrPool
    int32be m_restartCallback; //void(__cdecl* m_restartCallback)();
};

// Some extra unknown bytes
template<typename T>
struct poolState
{
    wiiP<T> objects; //void
    int32be free; //poolObject
    uint32be freeCount;
    uint32be totalCount;
    int32be unknown00;
};

//Similar to CList, made some helpers for itteration
template<typename T>
class CPool
{
public:
    T* GetObjectAtIndex(int Index)
    {
        if (Index <= GetTotalCount())
        {
            int indexOffset = sizeof(T) * Index;
            DWORD64 BaseItemAddy = (DWORD64)m_pool.objects.Get();

            DWORD64 ItemAddress = BaseItemAddy + (DWORD64)indexOffset;
            T* ret = (T*)ItemAddress;
            return ret;
        }
        return nullptr;
    }

    int GetFreeCount()
    {
        return m_pool.freeCount.Get();
    }

    int GetTotalCount()
    {
        return m_pool.totalCount.Get();
    }

private:
    poolState<T> m_pool;
};

// It is in the name bud
struct char16_s
{
    char chars[16];
};

// Standard vector3d
class vec3_u
{
public:
    floatbe x;
    floatbe y;
    floatbe z;
};

//Hand defind no clue on name

class ObjectInfo
{
public:
    floatbe ScaleX;
    char Unknown1[16];
    floatbe ScaleY;
    char Unknown2[16];
    floatbe ScaleZ;
    char Unknown3[4];
    vec3_u Location;
    char UnknownLocationEffect[4];
    char PaddingTo68[4];
    unsigned int unknownintPerchance;
};

//I dont have the slightest of a clue what this is exactly but thats okay =)
struct opaqueProp // sizeof=0x5C
{
    wiiP<ObjectInfo> meshObject;
    int32be dword4;
    byte gap8[24];
    floatbe ScaleX;
    char Unknown1[16];
    floatbe ScaleY;
    char Unknown2[16];
    floatbe ScaleZ;
    byte gap4C[4];
    vec3_u vec3_u50;
};

struct $1872053F6F063FB8907DA0F24D4F61F9
{
    uint16be index;
    unsigned __int8 group;
    unsigned __int8 flags;
};

union $1D971A06973327890790F6A0F7BB9443
{
    $1872053F6F063FB8907DA0F24D4F61F9 __s0;
    uint32be whole;
};

struct NetPropID
{
    $1D971A06973327890790F6A0F7BB9443 ___u0;
};

// I did not add in this whole class because I just dont feel like defining it all myself. They changed a lot so yeah. Should end at 0x9B though.
class CGameProp : public CBaseObject
{
public:
    enum ETickType
    {
        k_tickAlways = 0x0,
        k_tickNever = 0x1,
        k_numTickTypes = 0x2,
    };

public:
    char UndefinedData[32];
    char16_s m_id;
    char16_s m_creatingDofName;
    wiiP<opaqueProp> m_prop;
    uint32be m_ref;
    uint32be m_attachments;
    NetPropID m_netid;
    int32be m_tickType;
    unsigned int m_flags;
public:
    void SetPosition(const vec3_u* pos);
    void SetScale(float ScaleX, float ScaleY, float ScaleZ);
};

class __declspec(align(4)) TPropInfo
{
public:
    wiiP<CGameProp> prop;
    uint32be handle;
    int8_t inActiveList;
    char flags;
};

// Pool of props
class TPropMgrState
{
public:
    CPool<TPropInfo> GetPropTable()
    {
        return table;
    }
private:
    CPool<TPropInfo> table;
};

// General stuff setup to make some simple examples of game runtime logic
namespace FRD
{
    static CMgrMgr* Cmgr;
    static TPropMgrState* s_propMgrState;

    // Some things I defined out myself cause we need something for this stupid ass proof of concept
    static void InitFRDSDK()
    {
        int* mgrMgrPtr;
        mgrMgrPtr = ReadGameMemory<int>((DWORD)0x807754C0);

        auto fixedptr = int32_swap(*mgrMgrPtr);
        Cmgr = ReadGameMemory<CMgrMgr>((DWORD)fixedptr);

        s_propMgrState = ReadGameMemory<TPropMgrState>(0x80754E4C);
    }
};


/*
    Me after not touching battlefront for years but a new build just dropped and Corra is blowing up my dms
    https://vxtwitter.com/i/status/1870915020374343749
*/