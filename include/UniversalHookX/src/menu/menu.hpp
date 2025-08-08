#pragma once

#include <Windows.h>
#include "../dependencies/imgui/imgui.h"
#include <cstdint>

struct Weather
{
    uint32_t WeatherId = 0;
    bool Enable=true;
    float WindDirection_X = 1.0f;
    float WindDirection_Y = 0.0f;

    uint32_t WindLevel     = 4;
    uint32_t RainyLevel    = 0;
    uint32_t CloudyLevel   = 0;
    uint32_t SnowLevel     = 0;
    uint32_t FogLevel      = 0;
    uint32_t SandLevel     = 0;
    uint32_t ThunderLevel  = 0;
};

struct GameHack
{

    bool PerfectSubGame=true;
    uint32_t KeyMethod = 0; //0 key injections;  1 controller mimic; 
    float MiracleScale = 50.0f;
    float DamageMultiplier = 1.0f;
    bool Invincible =true;
    bool NoZeroItem = true;
};


namespace Menu {
    void InitializeContext(HWND hwnd);
    void Render( );

    inline bool bShowMenu = true;
    static const char* mainWindowName="Patapon+";


        //log window
    static ImGuiTextBuffer logBuffer;
    static ImGuiTextFilter logFilter;
    static bool scrollToBottom;
    static bool autoScroll = true;
    static const int TickSize=120;
    static bool autoAppendCommand = true;
    //use extern
    extern Weather WeatherSetting;
    extern GameHack GameHackSetting;
    extern uint8_t KeyConfig[8];
    void AddLog(const char* fmt, ...);
    void DrawKeyRow(const char* keys, int count, int CommandIndex);

} // namespace Menu
