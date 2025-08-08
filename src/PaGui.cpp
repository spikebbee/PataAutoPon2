

#include <stdio.h>
#include <PaGui.h>
#include <UniversalHookX\src\hooks\hooks.hpp>
#include <Detours/detours.h>

// Data

HWND PaGuiPlugin::PataponWnd;



bool PaGuiPlugin::Initialize(HWND window)
{

    PaGuiPlugin::PataponWnd=window;

    DetourRestoreAfterWith();
    DetourTransactionBegin();
    H::Init();
    if (DetourTransactionCommit() != ERROR_SUCCESS) return false;

    // VM_END

    return true;    

}



DWORD WINAPI PaGuiPlugin::RenderImGui(LPVOID lpParam)
{

    Initialize(static_cast<HWND>(lpParam));

    return 0;
}
