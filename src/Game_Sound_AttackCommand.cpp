#include <Patapon2Class.h>
#include <Detours/detours.h>
#include <UniversalHookX/src/menu/menu.hpp>


int Game_Sound_AttackCommand::KeyInjected = -1;


Game_Sound_AttackCommand* Game_Sound_AttackCommand::Instance = nullptr;

int32_t(__thiscall* Game_Sound_AttackCommand::Orig_fetchHit)(
    Game_Sound_AttackCommand*, void*, void*) = nullptr;

int32_t Game_Sound_AttackCommand::Hooked_fetchHit(void* pad, void* method)
{
    int32_t result = Orig_fetchHit(this, pad, method);

    if(result>0) return result;
    // Menu::AddLog(" @ fetchHit called: result = %d\n", result);

    //injection
    if(Menu::GameHackSetting.KeyMethod==0 && Game_Sound_AttackCommand::KeyInjected>0)
    {
        result=Game_Sound_AttackCommand::KeyInjected;
        Game_Sound_AttackCommand::KeyInjected=-1;
    };


    return result;
}

void Game_Sound_AttackCommand::Hook_Game_Sound_AttackCommand(uintptr_t dllBase)
{
    Orig_fetchHit = reinterpret_cast<decltype(Orig_fetchHit)>(dllBase + 0x05CAD60);
    Instance = new Game_Sound_AttackCommand();

    auto hook = &Game_Sound_AttackCommand::Hooked_fetchHit;
    DetourAttach(&reinterpret_cast<PVOID&>(Orig_fetchHit), reinterpret_cast<PVOID&>(hook));
}
