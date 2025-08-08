#include <Patapon.h>
#include <Detours/detours.h>
#include <ViGEmCon.h>
#include <chrono>
#include <thread>
#include <Patapon2Class.h>
#include <PaGui.h>
#include <UniversalHookX\src\menu\menu.hpp>


static WNDPROC originalWndProc = nullptr;
HWND Patapon::PaWnd = nullptr;


LRESULT CALLBACK HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int key = static_cast<int>(wParam);

    for (int i=0;i<8;i++)
    {
        if(key==Menu::KeyConfig[i])
        {
            //start signal
            if(Sound_BeatCommander::CommandStep==-100) Sound_BeatCommander::CommandStep=-1;

            //insert move
            if (uMsg == WM_KEYUP) Sound_BeatCommander::InsertCommand(key);
            
            //disable input
            return 0;
        } 
    }

    //original 
    return CallWindowProc(originalWndProc, hWnd, uMsg, wParam, lParam);
}



int32_t Patapon::Initialize()
{
    Menu::AddLog("* Inject Successful...\n");

    //-------------------------------------------------------------------
    //hook WndProc
    HWND hwnd = FindWindow(NULL, L"PATAPON 1+2 REPLAY");
    if (hwnd)
    {
        Patapon::PaWnd = hwnd;
        originalWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HookedWndProc)));

        if (originalWndProc)
            Menu::AddLog(" - WndProc hook installed!\n");
        else
            Menu::AddLog(" x SetWindowLongPtr failed. Error: %lu\n", GetLastError());
    }
    else
    {
        Menu::AddLog(" x FindWindow failed! Could not find 'PATAPON 1+2 REPLAY'\n");
    }

    //-------------------------------------------------------------------
    // Setup ViGEm once when the DLL is injected
    auto res = ViGEmControl::SetupViGEm();
    if (res)
    {
        Menu::AddLog(" - ViGEm installed.\n");
        
    }
    else
    {
        Menu::AddLog(" x ViGEm setup failed. use Key-injection as default\n");
        // return 1;
    }

    //-------------------------------------------------------------------
    Menu::AddLog("\n* Install Hooks\n");

    HMODULE hGameAssembly = nullptr;
    while (!(hGameAssembly = GetModuleHandleA("GameAssembly.dll")))
        Sleep(100);
    uintptr_t base = (uintptr_t)hGameAssembly;
    uintptr_t addr = base + 0x4E3D50;  // RVA of getNextBeatTickTime

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());


    Menu::AddLog(" - Hook Sound_BeatCommander.\n");
    Sound_BeatCommander::HookSound_BeatCommander(base);
    Menu::AddLog(" - Hook Game_Map_Weather_Controller.\n");
    Game_Map_Weather_Controller::HookGame_Map_Weather_Controller(base);
    Menu::AddLog(" - Hook Sound_SubGame_Script_Analyzer.\n");
    Sound_SubGame_Script_Analyzer::Hook_Sound_SubGame_Script_Analyzer(base);
    Menu::AddLog(" - Hook Game_Sound_AttackCommand.\n");
    Game_Sound_AttackCommand::Hook_Game_Sound_AttackCommand(base);
    Menu::AddLog(" - Hook Game_Miracle_Controller.\n");
    Game_Miracle_Controller::Hook_Game_Miracle_Controller(base);
    Menu::AddLog(" - Hook GameSystem_DamageCalculation.\n");
    GameSystem_DamageCalculation::Hook_GameSystem_DamageCalculation(base);
    Sound_SubGame_Scorer::Hook_Sound_SubGame_Scorer(base);
    Menu::AddLog(" - Hook GameSystem_Item.\n");
    GameSystem_Item_GlobalData::Hook_GameSystem_Item_GlobalData(base);
    GameSystem_Item_Operator::Hook_GameSystem_Item_Operator(base);
    Bases_Item_Slot::Hook_Bases_Item_Slot(base);
    Game_Unit_UnitTroop::Hook_Game_Unit_UnitTroop(base);
    GameSystem_Effect_Manager::Hook_GameSystem_Effect_Manager(base);


    // Game_Map_FogLayer::HookGame_Map_FogLayer(base);
    // Sound_SubGame_Miracle_Scorer::Hook_Sound_SubGame_Miracle_Scorer(base);
    // Sound_SubGame_Miracle_Command::Hook_Sound_SubGame_Miracle_Command(base);
    
    //-------------------------------------------------------------------

    Menu::AddLog("* Install GUI Hooks\n");
    PaGuiPlugin::RenderImGui(Patapon::PaWnd);


    LONG error = DetourTransactionCommit();
    // if (error != NO_ERROR) {
    //     Menu::AddLog(" x Some Detour Commit failed: %ld\n", error);
    // } else {
    //     Menu::AddLog(" - Successfully attached all hooks!\n");
    // }

    //-------------------------------------------------------------------
    Menu::AddLog("\n");Menu::AddLog("Ö Listening Command  \n");

    timeEndPeriod(1);
    return 0;
}
