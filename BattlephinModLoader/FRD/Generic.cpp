#include "Generic.h"
#include <iostream>

//Attempt to begin translating the actual function over from r7, works but not gonna use it
void __fastcall CGameProp::SetPosition(const vec3_u *pos)
{
    struct opaqueProp* Zm_prop; // r11
    int v3; // r11

    Zm_prop = m_prop.Get();
    Zm_prop->vec3_u50 = *pos;
    v3 = Zm_prop->dword4.Get();
    if (v3)
    {
        //*(vec3_u*)(v3 + 48) = *pos;
        //*(_DWORD*)(v3 + 68) |= 0x10000000u;
    }
    //this->m_flags |= 4u;
}