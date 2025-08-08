#include <Patapon2Class.h>
#include <Detours/detours.h>
#include <UniversalHookX/src/menu/menu.hpp>


void(__thiscall* Game_Miracle::Orig_Game_Miracle_Offense__start)(Game_Miracle*, float, const void*) = nullptr;
void(__thiscall* Game_Miracle::Orig_Game_Miracle_Quake__start)(Game_Miracle*, float, const void*) = nullptr;
void(__thiscall* Game_Miracle::Orig_Game_Miracle_Rain__start)(Game_Miracle*, float, const void*) = nullptr;
void(__thiscall* Game_Miracle::Orig_Game_Miracle_SnowStorm__start)(Game_Miracle*, float, const void*) = nullptr;
void(__thiscall* Game_Miracle::Orig_Game_Miracle_Storm__start)(Game_Miracle*, float, const void*) = nullptr;
void(__thiscall* Game_Miracle::Orig_Game_Miracle_Wind__start)(Game_Miracle*, float, const void*) = nullptr;
void(__thiscall* Game_Miracle::Orig_Game_Miracle_Quake__update)(Game_Miracle*, uint32_t, float, const void*) = nullptr;
void(__thiscall* Game_Miracle::Orig_Game_Miracle_Rain__update)(Game_Miracle*, uint32_t, float, const void*) = nullptr;


// Hooked methods
void Game_Miracle::Hooked_Game_Miracle_Offense__start(float time, const void* method) {
    Orig_Game_Miracle_Offense__start(this, time, method);
}
void Game_Miracle::Hooked_Game_Miracle_Quake__start(float time, const void* method) {
    Orig_Game_Miracle_Quake__start(this, time, method);
}
void Game_Miracle::Hooked_Game_Miracle_Rain__start(float time, const void* method) {
    Orig_Game_Miracle_Rain__start(this, time, method);
}
void Game_Miracle::Hooked_Game_Miracle_SnowStorm__start(float time, const void* method) {
    Orig_Game_Miracle_SnowStorm__start(this, time, method);
}
void Game_Miracle::Hooked_Game_Miracle_Storm__start(float time, const void* method) {
    Orig_Game_Miracle_Storm__start(this, time, method);
}
void Game_Miracle::Hooked_Game_Miracle_Wind__start(float time, const void* method) {
    Orig_Game_Miracle_Wind__start(this, time, method);
}

void Game_Miracle::Hooked_Game_Miracle_Quake__update(uint32_t dt, float timelimit, const void* method)
{

    Orig_Game_Miracle_Quake__update(this, dt, timelimit, method);
}

void Game_Miracle::Hooked_Game_Miracle_Rain__update(uint32_t dt, float timelimit, const void* method)
{
    // Menu::AddLog(" @ Rain::update called: dt=%u, limit=%.2f\n", dt, timelimit);
    timelimit = 10000.0f;
    Orig_Game_Miracle_Rain__update(this, dt, timelimit, method);
}





void Game_Miracle::Hook_Game_Miracle(uintptr_t dllBase)
{


    Orig_Game_Miracle_Offense__start   = reinterpret_cast<decltype(Orig_Game_Miracle_Offense__start)>(dllBase + 0x078A150);
    Orig_Game_Miracle_Quake__start     = reinterpret_cast<decltype(Orig_Game_Miracle_Quake__start)>(dllBase + 0x078A8A0);
    Orig_Game_Miracle_Rain__start      = reinterpret_cast<decltype(Orig_Game_Miracle_Rain__start)>(dllBase + 0x078A9A0);
    Orig_Game_Miracle_SnowStorm__start = reinterpret_cast<decltype(Orig_Game_Miracle_SnowStorm__start)>(dllBase + 0x078ACD0);
    Orig_Game_Miracle_Storm__start     = reinterpret_cast<decltype(Orig_Game_Miracle_Storm__start)>(dllBase + 0x078AEF0);
    Orig_Game_Miracle_Wind__start      = reinterpret_cast<decltype(Orig_Game_Miracle_Wind__start)>(dllBase + 0x078B190);

    Orig_Game_Miracle_Quake__update = reinterpret_cast<decltype(Orig_Game_Miracle_Quake__update)>(dllBase + 0x078A900);
    Orig_Game_Miracle_Rain__update  = reinterpret_cast<decltype(Orig_Game_Miracle_Rain__update)>(dllBase + 0x078AB60);



    auto Offense    = &Game_Miracle::Hooked_Game_Miracle_Offense__start  ;
    auto Quake      = &Game_Miracle::Hooked_Game_Miracle_Quake__start    ;
    auto Rain       = &Game_Miracle::Hooked_Game_Miracle_Rain__start     ;
    auto SnowStorm  = &Game_Miracle::Hooked_Game_Miracle_SnowStorm__start;
    auto Storm      = &Game_Miracle::Hooked_Game_Miracle_Storm__start    ;
    auto Wind       = &Game_Miracle::Hooked_Game_Miracle_Wind__start     ;

    auto Quake__update = &Game_Miracle::Hooked_Game_Miracle_Quake__update;
    auto Rain__update  = &Game_Miracle::Hooked_Game_Miracle_Rain__update ; 

    //not helpful
    // DetourAttach(&reinterpret_cast<PVOID&>(Orig_Game_Miracle_Offense__start),   reinterpret_cast<PVOID&>(Offense  ));
    // DetourAttach(&reinterpret_cast<PVOID&>(Orig_Game_Miracle_Quake__start),     reinterpret_cast<PVOID&>(Quake    ));
    // DetourAttach(&reinterpret_cast<PVOID&>(Orig_Game_Miracle_Rain__start),      reinterpret_cast<PVOID&>(Rain     ));
    // DetourAttach(&reinterpret_cast<PVOID&>(Orig_Game_Miracle_SnowStorm__start), reinterpret_cast<PVOID&>(SnowStorm));
    // DetourAttach(&reinterpret_cast<PVOID&>(Orig_Game_Miracle_Storm__start),     reinterpret_cast<PVOID&>(Storm    ));
    // DetourAttach(&reinterpret_cast<PVOID&>(Orig_Game_Miracle_Wind__start),      reinterpret_cast<PVOID&>(Wind     ));
    // DetourAttach(&reinterpret_cast<PVOID&>(Orig_Game_Miracle_Quake__update),    reinterpret_cast<PVOID&>(Quake__update ));
    // DetourAttach(&reinterpret_cast<PVOID&>(Orig_Game_Miracle_Rain__update),     reinterpret_cast<PVOID&>(Rain__update  ));

}




Game_Miracle_Controller* Game_Miracle_Controller::Instance = nullptr;

void(__thiscall* Game_Miracle_Controller::Orig_Game_Miracle_Controller__setup)(
    Game_Miracle_Controller*, int32_t, const void*) = nullptr;

void Game_Miracle_Controller::Hooked_Game_Miracle_Controller__setup(int32_t type, const void* method)
{

    Orig_Game_Miracle_Controller__setup(this, type, method);

    float* timeScale = (float*)((uintptr_t)this + 0x30);
    *timeScale = Menu::GameHackSetting.MiracleScale;
    
    Menu::AddLog("  @ MiracleController::setup: timeScale = %f\n", *timeScale);
    

}

void Game_Miracle_Controller::Hook_Game_Miracle_Controller(uintptr_t dllBase)
{
    Orig_Game_Miracle_Controller__setup =
        reinterpret_cast<decltype(Orig_Game_Miracle_Controller__setup)>(dllBase + 0x0789440);

    Instance = new Game_Miracle_Controller();

    auto hook = &Game_Miracle_Controller::Hooked_Game_Miracle_Controller__setup;
    DetourAttach(&reinterpret_cast<PVOID&>(Orig_Game_Miracle_Controller__setup), reinterpret_cast<PVOID&>(hook));
}