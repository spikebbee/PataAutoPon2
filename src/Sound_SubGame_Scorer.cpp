#include <Patapon2Class.h>
#include <Detours/detours.h>
#include <UniversalHookX/src/menu/menu.hpp>

Sound_SubGame_Scorer* Sound_SubGame_Scorer::Instance = nullptr;

float(__thiscall* Sound_SubGame_Scorer::Orig_getScore)(
    Sound_SubGame_Scorer*, const void*) = nullptr;

bool(__thiscall* Sound_SubGame_Scorer::Orig_isPerfect)(
    Sound_SubGame_Scorer*, const void*) = nullptr;

void(__thiscall* Sound_SubGame_Scorer::Orig_check)(
    Sound_SubGame_Scorer*, int32_t, float, const void*) = nullptr;

float Sound_SubGame_Scorer::Hooked_getScore(const void* method)
{
    auto res = Orig_getScore(this, method);
    if (Menu::GameHackSetting.PerfectSubGame)
    {
        Menu::AddLog("[Scorer] getScore %.1f overridden to 1.0\n", res);
        return 1.0f;
    }
    return res;
}

bool Sound_SubGame_Scorer::Hooked_isPerfect(const void* method)
{
    auto res = Orig_isPerfect(this, method);
    if (Menu::GameHackSetting.PerfectSubGame)
    {
        Menu::AddLog("[Scorer] isPerfect %i overridden to true\n", res);
        return true;
    }
    return res;
}

void Sound_SubGame_Scorer::Hooked_check(int32_t ret, float rate, const void* method)
{
    if (Menu::GameHackSetting.PerfectSubGame)
    {
        Menu::AddLog("[Scorer] check override: ret=%d, rate=%.2f → forcing perfect\n", ret, rate);
        // ret = 1;   // e.g., assume 1 means "OK" or "perfect"
        // rate = 1.0f;
    }

    Orig_check(this, ret, rate, method);
}

void Sound_SubGame_Scorer::Hook_Sound_SubGame_Scorer(uintptr_t dllBase)
{
    Orig_getScore = reinterpret_cast<decltype(Orig_getScore)>(dllBase + 0x0505270);
    auto Score = &Sound_SubGame_Scorer::Hooked_getScore;
    DetourAttach(&reinterpret_cast<PVOID&>(Orig_getScore), reinterpret_cast<PVOID&>(Score));

    Orig_isPerfect = reinterpret_cast<decltype(Orig_isPerfect)>(dllBase + 0x05052D0);
    auto Perfect = &Sound_SubGame_Scorer::Hooked_isPerfect;
    DetourAttach(&reinterpret_cast<PVOID&>(Orig_isPerfect), reinterpret_cast<PVOID&>(Perfect));

    // Orig_check = reinterpret_cast<decltype(Orig_check)>(dllBase + 0x0505180);
    // auto Check = &Sound_SubGame_Scorer::Hooked_check;
    // DetourAttach(&reinterpret_cast<PVOID&>(Orig_check), reinterpret_cast<PVOID&>(Check));
}
