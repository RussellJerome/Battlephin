#include "ExampleMod.h"
#include "Memory/Guest/Wii.h"
#include "Memory/Helper.h"
#include "FRD/Generic.h"
#include "GUI/ImGui/imgui.h"


// Only Called Once, if you need to hook shit, declare some global non changing values
void ExampleMod::InitializeMod()
{
    //Initialize the Dolphin Memory pointer for the mod. I may make this a static function in the future but it works for now
    UpdateMemoryStartLocations();
    //Just sets up 2 basic important Objects in bfiii, managers and prop array
    FRD::InitFRDSDK();
}

void ExampleMod::CleanUpMod()
{
	Mod::CleanUpMod();
}

void ExampleMod::DrawImGui()
{
    //Example of using Imgui to work with le mods

    ImGui::Begin("Example Mod");

    // All the game manager objects being displayed
    if (ImGui::CollapsingHeader("Game Managers"))
    {
        for (size_t i = 0; i < FRD::Cmgr->m_mgrs.GetCount(); i++)
        {
            MGRRecord* Record = FRD::Cmgr->m_mgrs.GetIndex<MGRRecord>(i);
            std::string nme(Record->dictKey.Get());
            if (ImGui::Button(nme.c_str()))
            {
                //Debugging test
                std::cout << nme << std::endl;
                std::cout << "Printing Address: " << (void*)Record->ManagerRefObject.GetRawPointer().Get() << std::endl;
            }
        }

    }

    // Displays all the current world Props and you can dump some info on it
    if (ImGui::CollapsingHeader("WorldProps"))
    {
        //ITTERATE ME HARDER
        for (size_t i = 0; i < FRD::s_propMgrState->GetPropTable().GetTotalCount(); i++)
        {
            auto propInfo = FRD::s_propMgrState->GetPropTable().GetObjectAtIndex(i);
            if (propInfo->flags == 0)
            {
                auto actualProp = propInfo->prop.Get();
                std::string nme(actualProp->m_id.chars);
                std::string ModLabel = nme + "##" + std::to_string(i);

                if (ImGui::TreeNode(ModLabel.c_str()))
                {
                    ImGui::Separator();
                    opaqueProp* opaquePropPtr = actualProp->m_prop.Get();
                    ImGui::Separator();
                    if (ImGui::Button("Print Class Info"))
                    {
                        std::cout << "Host Memory: " << actualProp << std::endl;
                        std::cout << "Guest Memory: " << (void*)propInfo->prop.GetRawPointer().Get() << std::endl;
                        std::cout << "GetClassIDString: " << actualProp->GetClassID() << std::endl;
                    }

                    /*
                    *    This code below is a proof of concept at editing positions and scale of CGameProps. 
                    *    It works but has several issues that I really don't give a fuck about solving because I just dont want to deal with the stupid fucking asm I have been staring at.
                    *    SETTING THE LOCATION DOESNT SEEM TO UPDATE THE PHYSICS FOR THE FUCKING MESH HALF THE TIME.
                    *    I WROTE THIS ENTIRE PROGRAM ON A BENDER AND I JUST DONT HAVE THE STRENGTH to do this anymore
                    */

                    float loc[3] = { opaquePropPtr->vec3_u50.x.Get(), opaquePropPtr->vec3_u50.y.Get(), opaquePropPtr->vec3_u50.z.Get() };
                    ImGui::DragFloat3("Location", loc, 0.10);
                    vec3_u* LocationNew = new vec3_u();
                    LocationNew->x.Set(loc[0]);
                    LocationNew->y.Set(loc[1]);
                    LocationNew->z.Set(loc[2]);
                    actualProp->SetPosition(LocationNew);

                    float scale[3] = { opaquePropPtr->ScaleX.Get(), opaquePropPtr->ScaleY.Get(), opaquePropPtr->ScaleZ.Get()};
                    ImGui::DragFloat3("Scale", scale, 0.10);
                    actualProp->SetScale(scale[0], scale[1], scale[2]);

                    ImGui::TreePop();
                }
            }


        }
    }

    //No repentance?
    if (ImGui::Button("The Rapture"))
    {
        for (size_t i = 0; i < FRD::Cmgr->m_mgrs.GetCount(); i++)
        {
            MGRRecord* Record = FRD::Cmgr->m_mgrs.GetIndex<MGRRecord>(i);
            if (std::string(Record->dictKey.Get()) == "havokphysicsmgr")
            {
                DWORD64 ManagerAddy = (DWORD64)Record->ManagerRefObject.Get();
                ManagerAddy = ManagerAddy + (DWORD64)0x3C; //Offset to m_gravityStrength
                floatbe* GravityDirection = (floatbe*)ManagerAddy;

                //You will meet god
                GravityDirection->Set(-20.0f);
            }
        }
    }

    ImGui::End();
}