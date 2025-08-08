#include <Patapon2Class.h>
#include <Detours/detours.h>
#include <UniversalHookX/src/menu/menu.hpp>

Game_Map_Weather_Controller* Game_Map_Weather_Controller::Game_Map_Weather_ControllerInstance = nullptr;

uint32_t Game_Map_Weather_Controller::LastWeatherSet = 0;
int32_t Game_Map_Weather_Controller::LastWeatherId = 0;
float Game_Map_Weather_Controller::LastTransitionTime = 0.0f;

// Definitions of static member function pointers
void (__thiscall* Game_Map_Weather_Controller::Orig_changeWeather)(Game_Map_Weather_Controller*, uint32_t, float, int32_t, const void*) = nullptr;
void (__thiscall* Game_Map_Weather_Controller::Orig_setWindLevel)(Game_Map_Weather_Controller*, uint32_t, int32_t, const void*) = nullptr;
void (__thiscall* Game_Map_Weather_Controller::Orig_setWindDirection)(Game_Map_Weather_Controller*, void*, int32_t, const void*) = nullptr;
void (__thiscall* Game_Map_Weather_Controller::Orig_setRainyLevel)(Game_Map_Weather_Controller*, uint32_t, int32_t, const void*) = nullptr;
void (__thiscall* Game_Map_Weather_Controller::Orig_setCloudyLevel)(Game_Map_Weather_Controller*, uint32_t, int32_t, const void*) = nullptr;
void (__thiscall* Game_Map_Weather_Controller::Orig_setSnowLevel)(Game_Map_Weather_Controller*, uint32_t, int32_t, const void*) = nullptr;
void (__thiscall* Game_Map_Weather_Controller::Orig_setFogLevel)(Game_Map_Weather_Controller*, uint32_t, int32_t, const void*) = nullptr;
void (__thiscall* Game_Map_Weather_Controller::Orig_setSandLevel)(Game_Map_Weather_Controller*, uint32_t, int32_t, const void*) = nullptr;
void (__thiscall* Game_Map_Weather_Controller::Orig_setThunderLevel)(Game_Map_Weather_Controller*, uint32_t, int32_t, const void*) = nullptr;

struct UnityEngine_Vector2_o {
    float x;
    float y;
};

// Hooked methods

void Game_Map_Weather_Controller::Hooked_changeWeather(uint32_t weather, float time, int32_t id, const void* method)
{
    Game_Map_Weather_ControllerInstance = this;
    LastWeatherSet = weather;
    LastWeatherId = id;
    LastTransitionTime = time;
    Menu::AddLog("  @ changeWeather: %u-> (id=%d, t=%.2f)\n", weather, Menu::WeatherSetting.WeatherId, id, time);
    
    if(Menu::WeatherSetting.Enable) weather=Menu::WeatherSetting.WeatherId;
    
    Orig_changeWeather(this, weather, time, id, method);
}

//level 0-4; direction (-1,0), (1,0)
void Game_Map_Weather_Controller::Hooked_setWindLevel(uint32_t level, int32_t id, const void* method)
{
    Menu::AddLog("  @ setWindLevel: %u->%u (id=%d)\n", level, Menu::WeatherSetting.WindLevel, id);

    level = Menu::WeatherSetting.WindLevel;

    Orig_setWindLevel(this, level, id, method);
}

void Game_Map_Weather_Controller::Hooked_setWindDirection(void* dir, int32_t id, const void* method)
{
    UnityEngine_Vector2_o* vec = reinterpret_cast<UnityEngine_Vector2_o*>(dir);
    if (vec) {
        // Menu::AddLog("  @ setWindDirection (id=%d): dir = (%.2f, %.2f)\n", id, vec->x, vec->y);
    } else {
        // Menu::AddLog("  @ setWindDirection (id=%d): dir = null\n", id);
    }

    vec->x = Menu::WeatherSetting.WindDirection_X;
    vec->y = Menu::WeatherSetting.WindDirection_Y;



    Orig_setWindDirection(this, dir, id, method);
}

void Game_Map_Weather_Controller::Hooked_setRainyLevel(uint32_t level, int32_t id, const void* method)
{
    Menu::AddLog("  @ setRainyLevel: %u->%u (id=%d)\n", level, Menu::WeatherSetting.RainyLevel, id);
    level = Menu::WeatherSetting.RainyLevel;

    Orig_setRainyLevel(this, level, id, method);
}

void Game_Map_Weather_Controller::Hooked_setCloudyLevel(uint32_t level, int32_t id, const void* method)
{
    Menu::AddLog("  @ setCloudyLevel: %u->%u (id=%d)\n", level, Menu::WeatherSetting.CloudyLevel, id);
    level = Menu::WeatherSetting.CloudyLevel;

    Orig_setCloudyLevel(this, level, id, method);
}

void Game_Map_Weather_Controller::Hooked_setSnowLevel(uint32_t level, int32_t id, const void* method)
{
    Menu::AddLog("  @ setSnowLevel: %u->%u (id=%d)\n", level, Menu::WeatherSetting.SnowLevel, id);
    level = Menu::WeatherSetting.SnowLevel;

    Orig_setSnowLevel(this, level, id, method);
}

void Game_Map_Weather_Controller::Hooked_setFogLevel(uint32_t level, int32_t id, const void* method)
{
    Menu::AddLog("  @ setFogLevel: %u->%u (id=%d)\n", level, Menu::WeatherSetting.FogLevel,id);
    level = Menu::WeatherSetting.FogLevel;
    Orig_setFogLevel(this, level, id, method);
}

void Game_Map_Weather_Controller::Hooked_setSandLevel(uint32_t level, int32_t id, const void* method)
{
    Menu::AddLog("  @ setSandLevel: %u->%u (id=%d)\n", level, Menu::WeatherSetting.SandLevel, id);
    level = Menu::WeatherSetting.SandLevel;

    Orig_setSandLevel(this, level, id, method);
}

void Game_Map_Weather_Controller::Hooked_setThunderLevel(uint32_t level, int32_t id, const void* method)
{
    Menu::AddLog("  @ setThunderLevel: %u->%u (id=%d)\n", level, Menu::WeatherSetting.ThunderLevel, id);
    level = Menu::WeatherSetting.ThunderLevel;

    Orig_setThunderLevel(this, level, id, method);
}

void Game_Map_Weather_Controller::HookGame_Map_Weather_Controller(uintptr_t dllBase)
{
    // Hook changeWeather
    Orig_changeWeather = reinterpret_cast<void(__thiscall*)(Game_Map_Weather_Controller*, uint32_t, float, int32_t, const void*)>(dllBase + 0x07CC7F0);
    auto changeWeather = &Game_Map_Weather_Controller::Hooked_changeWeather;
    DetourAttach(&reinterpret_cast<PVOID&>(Orig_changeWeather), reinterpret_cast<PVOID&>(changeWeather));

    // Hook setWindLevel
    Orig_setWindLevel = reinterpret_cast<void(__thiscall*)(Game_Map_Weather_Controller*, uint32_t, int32_t, const void*)>(dllBase + 0x07CCA90);
    auto setWindLevel = &Game_Map_Weather_Controller::Hooked_setWindLevel;
    DetourAttach(&reinterpret_cast<PVOID&>(Orig_setWindLevel), reinterpret_cast<PVOID&>(setWindLevel));

    // Hook setWindDirection
    Orig_setWindDirection = reinterpret_cast<void(__thiscall*)(Game_Map_Weather_Controller*, void*, int32_t, const void*)>(dllBase + 0x07CCCC0);
    auto setWindDirection = &Game_Map_Weather_Controller::Hooked_setWindDirection;
    DetourAttach(&reinterpret_cast<PVOID&>(Orig_setWindDirection), reinterpret_cast<PVOID&>(setWindDirection));

    // Hook setRainyLevel
    Orig_setRainyLevel = reinterpret_cast<void(__thiscall*)(Game_Map_Weather_Controller*, uint32_t, int32_t, const void*)>(dllBase + 0x07CCE20);
    auto setRainyLevel = &Game_Map_Weather_Controller::Hooked_setRainyLevel;
    DetourAttach(&reinterpret_cast<PVOID&>(Orig_setRainyLevel), reinterpret_cast<PVOID&>(setRainyLevel));

    // Hook setCloudyLevel
    Orig_setCloudyLevel = reinterpret_cast<void(__thiscall*)(Game_Map_Weather_Controller*, uint32_t, int32_t, const void*)>(dllBase + 0x07CD0B0);
    auto setCloudyLevel = &Game_Map_Weather_Controller::Hooked_setCloudyLevel;
    DetourAttach(&reinterpret_cast<PVOID&>(Orig_setCloudyLevel), reinterpret_cast<PVOID&>(setCloudyLevel));

    // Hook setSnowLevel
    Orig_setSnowLevel = reinterpret_cast<void(__thiscall*)(Game_Map_Weather_Controller*, uint32_t, int32_t, const void*)>(dllBase + 0x07CD3D0);
    auto setSnowLevel = &Game_Map_Weather_Controller::Hooked_setSnowLevel;
    DetourAttach(&reinterpret_cast<PVOID&>(Orig_setSnowLevel), reinterpret_cast<PVOID&>(setSnowLevel));

    // Hook setFogLevel
    Orig_setFogLevel = reinterpret_cast<void(__thiscall*)(Game_Map_Weather_Controller*, uint32_t, int32_t, const void*)>(dllBase + 0x07CD560);
    auto setFogLevel = &Game_Map_Weather_Controller::Hooked_setFogLevel;
    DetourAttach(&reinterpret_cast<PVOID&>(Orig_setFogLevel), reinterpret_cast<PVOID&>(setFogLevel));

    // Hook setSandLevel
    Orig_setSandLevel = reinterpret_cast<void(__thiscall*)(Game_Map_Weather_Controller*, uint32_t, int32_t, const void*)>(dllBase + 0x07CD6F0);
    auto setSandLevel = &Game_Map_Weather_Controller::Hooked_setSandLevel;
    DetourAttach(&reinterpret_cast<PVOID&>(Orig_setSandLevel), reinterpret_cast<PVOID&>(setSandLevel));

    //Hook setThunderLevel
    Orig_setThunderLevel = reinterpret_cast<decltype(Orig_setThunderLevel)>(dllBase + 0x07CD240);  
    auto ThunderLevel = &Game_Map_Weather_Controller::Hooked_setThunderLevel;
    DetourAttach(&reinterpret_cast<PVOID&>(Orig_setThunderLevel), reinterpret_cast<PVOID&>(ThunderLevel));

}
