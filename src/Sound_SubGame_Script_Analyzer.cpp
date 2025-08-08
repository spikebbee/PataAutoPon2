#include <Patapon2Class.h>
#include <Detours/detours.h>
#include <UniversalHookX/src/menu/menu.hpp>

Sound_SubGame_Script_Analyzer* Sound_SubGame_Script_Analyzer::Instance = nullptr;

int32_t(__thiscall* Sound_SubGame_Script_Analyzer::Orig_hitCheck)(Sound_SubGame_Script_Analyzer*, int32_t, const void*) = nullptr;
bool(__thiscall* Sound_SubGame_Script_Analyzer::Orig_isOkHit)(Sound_SubGame_Script_Analyzer*, const void*) = nullptr;
bool(__thiscall* Sound_SubGame_Script_Analyzer::Orig_isNgHit)(Sound_SubGame_Script_Analyzer*, const void*) = nullptr;

int32_t Sound_SubGame_Script_Analyzer::Hooked_hitCheck(int32_t percussion, const void* method)
{
    auto res = Orig_hitCheck(this, percussion, method);
    if (Menu::GameHackSetting.PerfectSubGame)
        return 1;
    return res;
}

bool Sound_SubGame_Script_Analyzer::Hooked_isOkHit(const void* method)
{
    auto res = Orig_isOkHit(this, method);
    Menu::AddLog("isOkHit %i\n",res);
    if (Menu::GameHackSetting.PerfectSubGame)
        return true;
    return res;
}

bool Sound_SubGame_Script_Analyzer::Hooked_isNgHit(const void* method)
{
    auto res = Orig_isNgHit(this, method);
    Menu::AddLog("isNgHit %i\n",res);

    if (Menu::GameHackSetting.PerfectSubGame)
        return false;
    return res;
}

void Sound_SubGame_Script_Analyzer::Hook_Sound_SubGame_Script_Analyzer(uintptr_t dllBase)
{
    Orig_hitCheck = reinterpret_cast<decltype(Orig_hitCheck)>(dllBase + 0x0536180);
    auto hitCheck = &Sound_SubGame_Script_Analyzer::Hooked_hitCheck;
    DetourAttach(&reinterpret_cast<PVOID&>(Orig_hitCheck),reinterpret_cast<PVOID&>(hitCheck));

    // Orig_isOkHit = reinterpret_cast<decltype(Orig_isOkHit)>(dllBase + 0x0536AF0);
    // auto isOkHit  = &Sound_SubGame_Script_Analyzer::Hooked_isOkHit ;
    // DetourAttach(&reinterpret_cast<PVOID&>(Orig_isOkHit),reinterpret_cast<PVOID&>(isOkHit ));

    // Orig_isNgHit = reinterpret_cast<decltype(Orig_isNgHit)>(dllBase + 0x0536B10);
    // auto isNgHit  = &Sound_SubGame_Script_Analyzer::Hooked_isNgHit ;
    // DetourAttach(&reinterpret_cast<PVOID&>(Orig_isNgHit),reinterpret_cast<PVOID&>(isNgHit ));
}
