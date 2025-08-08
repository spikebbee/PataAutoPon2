#include <ViGEmCon.h>
#include <windows.h>
#include <Patapon2Class.h>
#include <UniversalHookX\src\menu\menu.hpp>

static PVIGEM_CLIENT client = nullptr;
static PVIGEM_TARGET target = nullptr;
#include <cstdio>  

bool ViGEmControl::SetupViGEm()
{
    client = vigem_alloc();
    if (!client)
    {
        Menu::AddLog("Failed to allocate ViGEm client\n");
        return false;
    }

    if (vigem_connect(client) != VIGEM_ERROR_NONE)
    {
        Menu::AddLog(" x Failed to connect ViGEm\n");
        return false;
    }

    target = vigem_target_x360_alloc();
    if (!target)
    {
        Menu::AddLog(" x Failed to allocate X360 target\n");
        return false;
    }

    if (vigem_target_add(client, target) != VIGEM_ERROR_NONE)
    {
        Menu::AddLog(" x Failed to add virtual controller\n");
        return false;
    }

    Menu::AddLog(" - Virtual Xbox 360 controller added!\n");
    return true;
}
void ViGEmControl::SimulateX360Button(int button)
{

    //for signaling the gui 
    if(button==XUSB_GAMEPAD_Y) Sound_BeatCommander::lastMove =  4;
    if(button==XUSB_GAMEPAD_X) Sound_BeatCommander::lastMove =  3;
    if(button==XUSB_GAMEPAD_A) Sound_BeatCommander::lastMove =  2;
    if(button==XUSB_GAMEPAD_B) Sound_BeatCommander::lastMove =  1;
    Sound_BeatCommander::lastMoveTime = std::chrono::system_clock::now();




    switch (Menu::GameHackSetting.KeyMethod)
    {
        case 1:
            if (client && target)
            {
                XUSB_REPORT report = {};
                report.wButtons = button;

                // Press
                vigem_target_x360_update(client, target, report);
                Sleep(80); 
                // Release
                report.wButtons = 0;
                vigem_target_x360_update(client, target, report);
            }
        break;

        case 0:
            Sleep(30); 
            if(button==XUSB_GAMEPAD_Y) Game_Sound_AttackCommand::KeyInjected= 4;
            if(button==XUSB_GAMEPAD_X) Game_Sound_AttackCommand::KeyInjected= 3;
            if(button==XUSB_GAMEPAD_A) Game_Sound_AttackCommand::KeyInjected= 2;
            if(button==XUSB_GAMEPAD_B) Game_Sound_AttackCommand::KeyInjected= 1;
        break;

    };

}


void ViGEmControl::SimulateX360ButtonMira(int button)
{


        ViGEmControl::SimulateX360Button(button);
        Sleep(200); 
        ViGEmControl::SimulateX360Button(button);

        Sleep(480); 

        ViGEmControl::SimulateX360Button(button);
        Sleep(200); 
        ViGEmControl::SimulateX360Button(button);




    

}


