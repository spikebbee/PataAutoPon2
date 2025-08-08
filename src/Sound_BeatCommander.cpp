#include <Patapon2Class.h>
#include <Detours/detours.h>
#include <ViGEmCon.h>
#include <thread>
#include <UniversalHookX\src\menu\menu.hpp>

Sound_BeatCommander* Sound_BeatCommander::Sound_BeatCommanderInstance=nullptr;

int Sound_BeatCommander::last_tick  = -1;
int Sound_BeatCommander::tick_count = 0;
int Sound_BeatCommander::keyTiming = 80;
bool Sound_BeatCommander::MiracleNext = false;
const int Sound_BeatCommander::tickRecordSize=120;
std::vector<WORD> Sound_BeatCommander::KeySequence;
std::vector<uint32_t> Sound_BeatCommander::TickRecord;

int Sound_BeatCommander::lastMove = -1;
std::chrono::time_point<std::chrono::system_clock> Sound_BeatCommander::lastMoveTime;



int Sound_BeatCommander::CommandStep=-100;


uint32_t (__thiscall* Sound_BeatCommander::OrigGetNextBeatTickTime)(Sound_BeatCommander* pThis,const void* method);
void     (__thiscall* Sound_BeatCommander::OrigInitialize)         (Sound_BeatCommander* pThis, void* paramFileHolder, const void* method);
void     (__thiscall* Sound_BeatCommander::OrigUpdate)             (Sound_BeatCommander*, void*, uint32_t, const void*);
void     (__thiscall* Sound_BeatCommander::OrigClearHistroy)       (Sound_BeatCommander*, const void*);



void Sound_BeatCommander::HookedInitialize(void* paramFileHolder, const void* method)
{
    Menu::AddLog("  @ Sound_BeatCommander::initialize called.\n");
    OrigInitialize(this, paramFileHolder, method);
}


void Sound_BeatCommander::HookedClearHistroy(const void* method)
{
    Menu::AddLog("  @ Sound_BeatCommander::clearHistroy() called.\n");
    OrigClearHistroy(this, method);

    Sound_BeatCommander::CommandStep=-100;
    Sound_BeatCommander::KeySequence.clear();
}


WORD PrevMove[4];
void Sound_BeatCommander::HookedUpdate(void* pad, uint32_t dt, const void* method)
{
    Sound_BeatCommanderInstance = this;
    uint32_t tick = OrigGetNextBeatTickTime(this, method);
    
    // printf("[HOOK] BeatCommander::update called. dt = %u, pad = %p\n", dt, pad);
    // printf("  @ getNextBeatTickTime() => %u ; count  = %u\n", tick, tick_count);

    TickRecord.push_back(tick);

    while(TickRecord.size()>tickRecordSize)
    {
        TickRecord.erase(TickRecord.begin());
    }


    if(CommandStep>-5)
    {
        // send control
        if(tick==dt)
        {
            if(CommandStep>=0)
            {
                if (!KeySequence.empty()) 
                {


                    if(!MiracleNext)
                    {
                        ViGEmControl::SimulateX360Button(KeySequence[0]);
                        PrevMove[CommandStep]=KeySequence[0];
                        KeySequence.erase(KeySequence.begin());
                    }
                    else
                    {
                        if(CommandStep==0)  ViGEmControl::SimulateX360Button(XUSB_GAMEPAD_A );
                        if(CommandStep==1)  std::thread([] {ViGEmControl::SimulateX360ButtonMira(XUSB_GAMEPAD_A);}).detach();
                        if(CommandStep==3) MiracleNext=false;

                    }


                }
            }
            CommandStep++;
        }
        if(CommandStep==4) CommandStep=-4;
        if(CommandStep==-1)
        {
            //auto append last move
            if (KeySequence.empty()) 
            {
                // Menu::AddLog("Ö ");
                Sound_BeatCommander::KeySequence.push_back(PrevMove[0]);
                Sound_BeatCommander::KeySequence.push_back(PrevMove[1]);
                Sound_BeatCommander::KeySequence.push_back(PrevMove[2]);
                Sound_BeatCommander::KeySequence.push_back(PrevMove[3]);
            }
        }

    }


    OrigUpdate(this, pad, dt, method);
}


uint32_t Sound_BeatCommander::HookedGetNextBeatTickTime(const void* method)
{
    uint32_t tick = OrigGetNextBeatTickTime(this, method);
    return tick;
}

uint32_t Sound_BeatCommander::GetNextBeatTickTime()
{
    return OrigGetNextBeatTickTime(Sound_BeatCommanderInstance, 0);
}


void Sound_BeatCommander::InsertCommand(int move)
{

    auto size = Sound_BeatCommander::KeySequence.size();

    if(move == Menu::KeyConfig[7])
    {
        Menu::AddLog("\n");Menu::AddLog("   + Stop and Clear Sequence\n   ");
        Sound_BeatCommander::CommandStep=-100;
        Sound_BeatCommander::KeySequence.clear();
    }

    if(size%4!=0)
    {
        Menu::AddLog("\n");Menu::AddLog("   x Currently In Beats, Try Again\n   ");
        return;
    } 
    Sound_BeatCommander::KeySequence.clear();
    
    {
            if(move == Menu::KeyConfig[0])
            {
                Menu::AddLog("\n");Menu::AddLog("   + Forward [X][X][X][B]\n   ");

                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_X);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_X);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_X);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_B);
            }

            if(move == Menu::KeyConfig[1])
            {

                Menu::AddLog("\n");Menu::AddLog("   + Attack  [B][B][X][B]\n   ");

                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_B);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_B);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_X);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_B);
            }
            
            if(move == Menu::KeyConfig[2])
            {
                Menu::AddLog("\n");Menu::AddLog("   + Defend  [Y][Y][X][B]\n   ");

                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_Y);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_Y);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_X);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_B);
            }


            if(move == Menu::KeyConfig[3])
            {
                Menu::AddLog("\n");Menu::AddLog("   + Retreat [B][X][B][X]\n   ");

                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_B);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_X);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_B);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_X);
            }

            if(move == Menu::KeyConfig[4])
            {
                Menu::AddLog("\n");Menu::AddLog("   + Charge  [B][B][Y][Y]\n   ");

                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_B);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_B);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_Y);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_Y);
            }

            if(move == Menu::KeyConfig[5])
            {
                Menu::AddLog("\n");Menu::AddLog("   + Jump [A][A][Y][Y]\n   ");

                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_A);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_A);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_Y);
                Sound_BeatCommander::KeySequence.push_back(XUSB_GAMEPAD_Y);
            }

            if(move == Menu::KeyConfig[6])
            {
                Menu::AddLog("\n");Menu::AddLog("   + Miracle  [A][AA][AA]\n   ");
                Sound_BeatCommander::MiracleNext = true;
            }

    }
}


void Sound_BeatCommander::HookSound_BeatCommander(uintptr_t dllBase)
{
    // Hook getNextBeatTickTime
    OrigGetNextBeatTickTime = reinterpret_cast<uint32_t(__thiscall*)(Sound_BeatCommander*, const void*)>(dllBase + 0x4E3D50);
    auto NextBeatTickTime = &Sound_BeatCommander::HookedGetNextBeatTickTime;
    DetourAttach(&reinterpret_cast<PVOID&>(OrigGetNextBeatTickTime), reinterpret_cast<PVOID&>(NextBeatTickTime));

    // Hook initialize
    OrigInitialize = reinterpret_cast<void(__thiscall*)(Sound_BeatCommander*, void*, const void*)>(dllBase + 0x4DFE20);
    auto Initialize = &Sound_BeatCommander::HookedInitialize;
    DetourAttach(&reinterpret_cast<PVOID&>(OrigInitialize), reinterpret_cast<PVOID&>(Initialize));


    // Hook update
    OrigUpdate = reinterpret_cast<void(__thiscall*)(Sound_BeatCommander*, void*, uint32_t, const void*)>(dllBase + 0x4E0380);
    auto Update = &Sound_BeatCommander::HookedUpdate;
    DetourAttach(&reinterpret_cast<PVOID&>(OrigUpdate), reinterpret_cast<PVOID&>(Update));

//    // Hook terminate
//     OrigTerminate = reinterpret_cast<void(__thiscall*)(Sound_BeatCommander*, const void*)>(dllBase + 0x4E00E0);
//     auto Terminate = &Sound_BeatCommander::HookedTerminate;
//     DetourAttach(&reinterpret_cast<PVOID&>(OrigTerminate), reinterpret_cast<PVOID&>(Terminate));

//     // Hook endSubGame
//     OrigEndSubGame = reinterpret_cast<void(__stdcall*)(Sound_BeatCommander*, int32_t, const void*)>(dllBase + 0x4E4020);
//     auto EndSubGame = &Sound_BeatCommander::HookedEndSubGame;
//     DetourAttach(&reinterpret_cast<PVOID&>(OrigEndSubGame), reinterpret_cast<PVOID&>(EndSubGame));


    // Hook clearHistroy
    OrigClearHistroy = reinterpret_cast<void(__thiscall*)(Sound_BeatCommander*, const void*)>(dllBase + 0x4E07A0);
    auto ClearHistroy = &Sound_BeatCommander::HookedClearHistroy;
    DetourAttach(&reinterpret_cast<PVOID&>(OrigClearHistroy), reinterpret_cast<PVOID&>(ClearHistroy));


}
