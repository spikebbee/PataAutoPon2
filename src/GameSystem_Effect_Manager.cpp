#include <Patapon2Class.h>
#include <types.h>
#include <Detours/detours.h>
#include <UniversalHookX/src/menu/menu.hpp>


// -------------------- defs --------------------


void* (__stdcall* GameSystem_Effect_Manager::Orig_generateItemEffect)(
    void*, int32_t, uint32_t, void*, void*, float, const void*) = nullptr;



void* __stdcall GameSystem_Effect_Manager::Hooked_generateItemEffect(
    void* pThis,
    int32_t itemId,
    uint32_t effectId,
    void* position,
    void* velocity,
    float time,
    const void* method)
{
    float* posX = reinterpret_cast<float*>((uint8_t*)position + 0x0);
    float* posY = reinterpret_cast<float*>((uint8_t*)position + 0x4);
    auto now =duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    //more recent position, and drop itemId
    if(itemId>0 && Game_Unit_UnitTroop::AverageUnitX>300.0f && now-Game_Unit_UnitTroop::UnitX_Time<1000 && Menu::GameHackSetting.DropsOnYourHead)
    {
        Menu::AddLog("  @ EffectMgr generateItemEffect: drop item %d, Position Changed %.2f -> %.2f) \n",itemId,*posX,Game_Unit_UnitTroop::AverageUnitX);
        *posX = Game_Unit_UnitTroop::AverageUnitX-10.0f;


    }



    auto ret = Orig_generateItemEffect(pThis, itemId, effectId, position, velocity, time, method);
    return ret;
}

void GameSystem_Effect_Manager::Hook_GameSystem_Effect_Manager(uintptr_t dllBase)
{
    // Absolute shown: 0x00000001805B3660 -> offset from base is 0x05B3660
    Orig_generateItemEffect = reinterpret_cast<decltype(Orig_generateItemEffect)>(dllBase + 0x05B3660);
    DetourAttach(&(PVOID&)Orig_generateItemEffect, Hooked_generateItemEffect);
}