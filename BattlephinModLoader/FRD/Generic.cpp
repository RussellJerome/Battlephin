#include "Generic.h"
#include <iostream>

//Attempt to begin translating the actual function over from r7, works but not gonna use it
void __fastcall CGameProp::SetPosition(const vec3_u *pos)
{
    struct opaqueProp* Zm_prop; // r11

    Zm_prop = m_prop.Get();
    Zm_prop->vec3_u50 = *pos;
    auto v3 = Zm_prop->meshObject.Get();
    if (v3)
    {
        v3->Location = *pos;
        v3->unknownintPerchance |= 0x10000000u;
    }
    this->m_flags |= 4u;
}

void CGameProp::SetScale(float ScaleX, float ScaleY, float ScaleZ)
{
    auto Zm_prop = m_prop.Get();
    Zm_prop->ScaleX.Set(ScaleX);
    Zm_prop->ScaleY.Set(ScaleY);
    Zm_prop->ScaleZ.Set(ScaleZ);
    auto v3 = Zm_prop->meshObject.Get();
    if (v3)
    {
        v3->ScaleX.Set(ScaleX);
        v3->ScaleY.Set(ScaleY);
        v3->ScaleZ.Set(ScaleZ);
    }
}
