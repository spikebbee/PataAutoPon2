#pragma once

#include <windows.h>


#pragma pack(push, 1)
class PaGuiPlugin
{

    PaGuiPlugin() = default;


private:

public:

    static HWND PataponWnd;
    static bool Initialize(HWND window);
    static DWORD WINAPI RenderImGui(LPVOID lpParam);

};


#pragma pack(pop)