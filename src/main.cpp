

#include <windows.h>
#include <cstdio>      
#include "Patapon.h"  


DWORD WINAPI InstallHook(LPVOID lpParam)
{
    Patapon::Initialize();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            AllocConsole();
            AttachConsole(GetCurrentProcessId());
            FILE* fp;
            freopen_s(&fp, "CONOUT$", "w", stdout);
            SetConsoleOutputCP(CP_UTF8);     
            SetConsoleTitleA("Debug Console");
            CreateThread(nullptr, 0, InstallHook, nullptr, 0, nullptr);
            break;

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            // Optional: FreeConsole();
            break;
    }
    return TRUE;
}
