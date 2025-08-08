#include <Patapon2Class.h>
#include <Detours/detours.h>
#include <UniversalHookX/src/menu/menu.hpp>


// Static definitions
Sound_SubGame_Miracle_Command* Sound_SubGame_Miracle_Command::Instance = nullptr;

int32_t(__thiscall* Sound_SubGame_Miracle_Command::Orig_addCount)(
    Sound_SubGame_Miracle_Command* pThis,
    const void* method) = nullptr;
bool(__thiscall* Sound_SubGame_Miracle_Command::Orig_hitKey)(Sound_SubGame_Miracle_Command*, int32_t, bool, float, const void*) = nullptr;


int32_t Sound_SubGame_Miracle_Command::Hooked_addCount(const void* method)
{
    int32_t result = Orig_addCount(this, method);
    Menu::AddLog(" @ addCount called  result = %d \n", result);
    return result;
}

bool Sound_SubGame_Miracle_Command::Hooked_hitKey(int32_t percussion, bool gonext, float rate, const void* method)
{
    bool result = Orig_hitKey(this, percussion, gonext, rate, method);
    if(percussion>-1)Menu::AddLog(" @ hitKey(percussion=%d, gonext=%d, rate=%.2f)  %s \n", percussion, gonext, rate, result ? "true" : "false");
    //  return true;
    return result;
}

void Sound_SubGame_Miracle_Command::Hook_Sound_SubGame_Miracle_Command(uintptr_t dllBase)
{
    Orig_addCount = reinterpret_cast<decltype(Orig_addCount)>(dllBase + 0x050D9A0); 
    auto hookFunc = &Sound_SubGame_Miracle_Command::Hooked_addCount;
    // DetourAttach(&reinterpret_cast<PVOID&>(Orig_addCount), reinterpret_cast<PVOID&>(hookFunc));

    
    Orig_hitKey = reinterpret_cast<decltype(Orig_hitKey)>(dllBase + 0x050DD50);
    auto hitKeyHook = &Sound_SubGame_Miracle_Command::Hooked_hitKey;
    // DetourAttach(&reinterpret_cast<PVOID&>(Orig_hitKey), reinterpret_cast<PVOID&>(hitKeyHook));

}