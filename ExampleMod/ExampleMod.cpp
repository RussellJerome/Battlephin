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
                //std::cout << "Printing Address: " << (void*)Record->ManagerRefObject.GetRawPointer().Get() << std::endl;
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
                    *    This code below is a proof of concept at editing positions of CGameProps. 
                    *    It works but has several issues that I really don't give a fuck about solving because I just dont want to deal with the stupid fucking asm I have been staring at.
                    *    SETTING THE LOCATION DOESNT SEEM TO UPDATE THE PHYSICS OR COMPONENT RESPONSIBLE FOR DISPLAYING A ACTUAL FUCKING MESH HALF THE TIME.
                    *    I WROTE THIS ENTIRE PROGRAM ON A BENDER AND I JUST DONT HAVE THE STRENGTH to do this anymore
                    */

                    float loc[3] = { opaquePropPtr->vec3_u50.x.Get(), opaquePropPtr->vec3_u50.y.Get(), opaquePropPtr->vec3_u50.z.Get() };
                    auto buttonname = ModLabel + "LocoGo";

                    //UwU drag me daddy
                    ImGui::DragFloat3(buttonname.c_str(), loc);
                    opaquePropPtr->vec3_u50.x.Set(loc[0]);
                    opaquePropPtr->vec3_u50.y.Set(loc[1]);
                    opaquePropPtr->vec3_u50.z.Set(loc[2]);
                    ImGui::TreePop();
                }
            }


        }


    }
    ImGui::End();
}