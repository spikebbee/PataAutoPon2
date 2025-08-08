#include <Patapon2Class.h>
#include <Detours/detours.h>
#include <UniversalHookX/src/menu/menu.hpp>

// Static variable definitions
Game_Map_FogLayer* Game_Map_FogLayer::Game_Map_FogLayerInstance = nullptr;
void(__thiscall* Game_Map_FogLayer::Orig_setFogLevel)(Game_Map_FogLayer*, uint32_t, const void*) = nullptr;

void Game_Map_FogLayer::Hooked_setFogLevel(uint32_t level, const void* method)
{
    Menu::AddLog("  @ Game_Map_FogLayer::setFogLevel: %u -> %u\n", level, Menu::WeatherSetting.FogLevel);
    level = Menu::WeatherSetting.FogLevel;

    Orig_setFogLevel(this, level, method);
}

void Game_Map_FogLayer::HookGame_Map_FogLayer(uintptr_t dllBase)
{
    // Hook setFogLevel
    Orig_setFogLevel = reinterpret_cast<decltype(Orig_setFogLevel)>(dllBase + 0x078D150);
    auto FogLevel = &Game_Map_FogLayer::Hooked_setFogLevel;

    //not needed
    // DetourAttach(&reinterpret_cast<PVOID&>(Orig_setFogLevel), reinterpret_cast<PVOID&>(FogLevel));

}
