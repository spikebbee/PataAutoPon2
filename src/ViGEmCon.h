#pragma once
#include <windows.h>

#include <ViGEm/Client.h>

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "winmm.lib")


namespace ViGEmControl
{
    bool SetupViGEm();
    void SimulateX360Button(int button);
    void SimulateX360ButtonMira(int button);


};