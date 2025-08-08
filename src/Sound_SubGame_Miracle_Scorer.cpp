#include <Patapon2Class.h>
#include <Detours/detours.h>
#include <UniversalHookX/src/menu/menu.hpp>


// Static member definitions
Sound_SubGame_Miracle_Scorer* Sound_SubGame_Miracle_Scorer::Instance = nullptr;
float(__thiscall* Sound_SubGame_Miracle_Scorer::Orig_getScore)(
    Sound_SubGame_Miracle_Scorer* pThis,
    const void* method) = nullptr;

float Sound_SubGame_Miracle_Scorer::Hooked_getScore(const void* method)
{
    float score = Orig_getScore(this, method);
    Menu::AddLog(" @ getScore = %.3f", score);

    score=1.0f;

    return score;
}

void Sound_SubGame_Miracle_Scorer::Hook_Sound_SubGame_Miracle_Scorer(uintptr_t dllBase)
{
    Orig_getScore = reinterpret_cast<decltype(Orig_getScore)>(dllBase + 0x0513610); //
    auto hookFunc = &Sound_SubGame_Miracle_Scorer::Hooked_getScore;

    DetourAttach(&reinterpret_cast<PVOID&>(Orig_getScore), reinterpret_cast<PVOID&>(hookFunc));
}
