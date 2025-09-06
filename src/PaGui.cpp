

#include <stdio.h>
#include <PaGui.h>
#include <UniversalHookX\src\hooks\hooks.hpp>
#include <Detours/detours.h>

// Data

HWND PaGuiPlugin::PataponWnd;



bool PaGuiPlugin::Initialize(HWND window)
{

    PaGuiPlugin::PataponWnd=window;    
    H::Init();
    return true;    

}



DWORD WINAPI PaGuiPlugin::RenderImGui(LPVOID lpParam)
{

    Initialize(static_cast<HWND>(lpParam));

    return 0;
}
