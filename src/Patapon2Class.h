#pragma once
#include <windows.h>
#include <cstdint>
#include <cstdio>  
#include <vector>  
#include <chrono>
#include <map>
#include <utility>  
#include <types.h>
using namespace std::chrono;

class Sound_BeatCommander
{
    Sound_BeatCommander() = default;

private:
    uint32_t HookedGetNextBeatTickTime(const void* method);
    void HookedInitialize(void* paramFileHolder, const void* method);
    void HookedUpdate(void* pad, uint32_t dt, const void* method);
    void HookedClearHistroy(const void* method);


    static uint32_t(__thiscall* OrigGetNextBeatTickTime)(Sound_BeatCommander* pThis,const void* method);
    static void(__thiscall* OrigInitialize)(Sound_BeatCommander* pThis, void* paramFileHolder, const void* method);
    static void(__thiscall* OrigUpdate)(Sound_BeatCommander* pThis, void* pad, uint32_t dt, const void* method);
    static void (__thiscall* OrigClearHistroy)(Sound_BeatCommander*, const void* method);

public:

    static Sound_BeatCommander* Sound_BeatCommanderInstance;
    static void HookSound_BeatCommander(uintptr_t dllBase);
    static uint32_t GetNextBeatTickTime();
    static void InsertCommand(int move);


    static int last_tick;
    static int tick_count;
    static int keyTiming;
    static std::vector<WORD> KeySequence;
    static int CommandStep;
    static std::vector<uint32_t> TickRecord;
    static const int tickRecordSize;
    static bool MiracleNext;

    static int lastMove;
    static std::chrono::time_point<std::chrono::system_clock> lastMoveTime;

};

class Game_Map_Weather_Controller
{
    Game_Map_Weather_Controller() = default;

private:
    void Hooked_changeWeather(uint32_t weather, float time, int32_t id, const void* method);
    void Hooked_setWindDirection(void* dir, int32_t id, const void* method);
    void Hooked_setWindLevel(uint32_t level, int32_t id, const void* method);
    void Hooked_setRainyLevel(uint32_t level, int32_t id, const void* method);
    void Hooked_setCloudyLevel(uint32_t level, int32_t id, const void* method);
    void Hooked_setSnowLevel(uint32_t level, int32_t id, const void* method);
    void Hooked_setFogLevel(uint32_t level, int32_t id, const void* method);
    void Hooked_setSandLevel(uint32_t level, int32_t id, const void* method);
    void Hooked_setThunderLevel(uint32_t level, int32_t id, const void* method);

    static void(__thiscall* Orig_changeWeather)   (Game_Map_Weather_Controller* pThis, uint32_t weather, float time, int32_t id, const void* method);
    static void(__thiscall* Orig_setWindLevel)    (Game_Map_Weather_Controller* pThis, uint32_t level, int32_t id, const void* method);
    static void(__thiscall* Orig_setWindDirection)(Game_Map_Weather_Controller* pThis, void* dir, int32_t id, const void* method);
    static void(__thiscall* Orig_setRainyLevel)   (Game_Map_Weather_Controller* pThis, uint32_t level, int32_t id, const void* method);
    static void(__thiscall* Orig_setCloudyLevel)  (Game_Map_Weather_Controller* pThis, uint32_t level, int32_t id, const void* method);
    static void(__thiscall* Orig_setSnowLevel)    (Game_Map_Weather_Controller* pThis, uint32_t level, int32_t id, const void* method);
    static void(__thiscall* Orig_setFogLevel)     (Game_Map_Weather_Controller* pThis, uint32_t level, int32_t id, const void* method);
    static void(__thiscall* Orig_setSandLevel)    (Game_Map_Weather_Controller* pThis, uint32_t level, int32_t id, const void* method);
    static void (__thiscall* Orig_setThunderLevel)(Game_Map_Weather_Controller* pThis, uint32_t level, int32_t id, const void* method);

public:
    static Game_Map_Weather_Controller* Game_Map_Weather_ControllerInstance;
    static void HookGame_Map_Weather_Controller(uintptr_t dllBase);

    static uint32_t LastWeatherSet;
    static int32_t LastWeatherId;
    static float LastTransitionTime;
};


class Game_Map_FogLayer
{
    Game_Map_FogLayer() = default;

private:
    void Hooked_setFogLevel(uint32_t level, const void* method);
    static void(__thiscall* Orig_setFogLevel)(Game_Map_FogLayer* pThis, uint32_t level, const void* method);

public:
    static Game_Map_FogLayer* Game_Map_FogLayerInstance;
    static void HookGame_Map_FogLayer(uintptr_t dllBase);
};


class Sound_SubGame_Miracle_Scorer
{
    Sound_SubGame_Miracle_Scorer() = default;

private:
    float Hooked_getScore(const void* method);
    static float(__thiscall* Orig_getScore)(Sound_SubGame_Miracle_Scorer* pThis, const void* method);

public:
    static Sound_SubGame_Miracle_Scorer* Instance;
    static void Hook_Sound_SubGame_Miracle_Scorer(uintptr_t dllBase);
};

class Sound_SubGame_Miracle_Command
{
    Sound_SubGame_Miracle_Command() = default;

private:
    int32_t Hooked_addCount(const void* method);
    bool Hooked_hitKey(int32_t percussion, bool gonext, float rate, const void* method);

    static int32_t(__thiscall* Orig_addCount)(Sound_SubGame_Miracle_Command* pThis, const void* method);
    static bool(__thiscall* Orig_hitKey)(Sound_SubGame_Miracle_Command* pThis, int32_t percussion, bool gonext, float rate, const void* method);

public:
    static Sound_SubGame_Miracle_Command* Instance;
    static void Hook_Sound_SubGame_Miracle_Command(uintptr_t dllBase);
};


class Sound_SubGame_Script_Analyzer
{
    Sound_SubGame_Script_Analyzer() = default;

private:
    int32_t Hooked_hitCheck(int32_t percussion, const void* method);

    static int32_t(__thiscall* Orig_hitCheck)(
        Sound_SubGame_Script_Analyzer* pThis,
        int32_t percussion,
        const void* method);


    bool Hooked_isOkHit(const void* method);
    static bool(__thiscall* Orig_isOkHit)(Sound_SubGame_Script_Analyzer*, const void*);

    bool Hooked_isNgHit(const void* method);
    static bool(__thiscall* Orig_isNgHit)(Sound_SubGame_Script_Analyzer*, const void*);



public:
    static Sound_SubGame_Script_Analyzer* Instance;
    static void Hook_Sound_SubGame_Script_Analyzer(uintptr_t dllBase);
};

class Game_Sound_AttackCommand
{
    Game_Sound_AttackCommand() = default;

private:
    int32_t Hooked_fetchHit(void* pad, void* method);

    static int32_t(__thiscall* Orig_fetchHit)(
        Game_Sound_AttackCommand* pThis,
        void* pad,
        void* method);

public:
    static Game_Sound_AttackCommand* Instance;
    static void Hook_Game_Sound_AttackCommand(uintptr_t dllBase);
    static int KeyInjected;
};



class Game_Miracle
{
    Game_Miracle() = default;

private:
    void Hooked_Game_Miracle_Offense__start(float time, const void* method);
    void Hooked_Game_Miracle_Quake__start(float time, const void* method);
    void Hooked_Game_Miracle_Rain__start(float time, const void* method);
    void Hooked_Game_Miracle_SnowStorm__start(float time, const void* method);
    void Hooked_Game_Miracle_Storm__start(float time, const void* method);
    void Hooked_Game_Miracle_Wind__start(float time, const void* method);

    void Hooked_Game_Miracle_Quake__update(uint32_t dt, float timelimit, const void* method);
    void Hooked_Game_Miracle_Rain__update(uint32_t dt, float timelimit, const void* method);


    static void(__thiscall* Orig_Game_Miracle_Offense__start  )(Game_Miracle*, float, const void*);
    static void(__thiscall* Orig_Game_Miracle_Quake__start    )(Game_Miracle*, float, const void*);
    static void(__thiscall* Orig_Game_Miracle_Rain__start     )(Game_Miracle*, float, const void*);
    static void(__thiscall* Orig_Game_Miracle_SnowStorm__start)(Game_Miracle*, float, const void*);
    static void(__thiscall* Orig_Game_Miracle_Storm__start    )(Game_Miracle*, float, const void*);
    static void(__thiscall* Orig_Game_Miracle_Wind__start     )(Game_Miracle*, float, const void*);
    
    static void(__thiscall* Orig_Game_Miracle_Quake__update)(Game_Miracle*, uint32_t, float, const void*);
    static void(__thiscall* Orig_Game_Miracle_Rain__update)(Game_Miracle*, uint32_t, float, const void*);


public:

    static void Hook_Game_Miracle(uintptr_t dllBase);
};



class Game_Miracle_Controller
{
    Game_Miracle_Controller() = default;

private:
    void Hooked_Game_Miracle_Controller__setup(int32_t type, const void* method);

    static void(__thiscall* Orig_Game_Miracle_Controller__setup)(
        Game_Miracle_Controller* pThis,
        int32_t type,
        const void* method);

public:
    static Game_Miracle_Controller* Instance;
    static void Hook_Game_Miracle_Controller(uintptr_t dllBase);
};

class GameSystem_DamageCalculation
{
    GameSystem_DamageCalculation() = default;

private:
    static bool __stdcall Hooked_calcAttackPower(
        void* pHitInfo,
        void* pAttackDamageParam,
        const void* method);

    static bool(__stdcall* Orig_calcAttackPower)(
        void* pHitInfo,
        void* pAttackDamageParam,
        const void* method);

    static bool __stdcall Hooked_calcDamage(
        void* pHitInfo,
        void** pHitResult,
        void* pDamageParam,
        void* pGameStatus,
        void* pNbVec,
        float accuracyRatio,
        bool isShield,
        void* pAscActorObj,
        bool isEnemy,
        bool isGimmick,
        bool isMulti,
        const void* method);

    static bool(__stdcall* Orig_calcDamage)(
        void* pHitInfo,
        void** pHitResult,
        void* pDamageParam,
        void* pGameStatus,
        void* pNbVec,
        float accuracyRatio,
        bool isShield,
        void* pAscActorObj,
        bool isEnemy,
        bool isGimmick,
        bool isMulti,
        const void* method);

public:
    static void Hook_GameSystem_DamageCalculation(uintptr_t dllBase);
};

class Sound_SubGame_Scorer
{
public:
    static Sound_SubGame_Scorer* Instance;

    static void Hook_Sound_SubGame_Scorer(uintptr_t dllBase);

private:
    // Hook targets (original function pointers)
    static float(__thiscall* Orig_getScore)(Sound_SubGame_Scorer* pThis, const void* method);
    static bool(__thiscall* Orig_isPerfect)(Sound_SubGame_Scorer* pThis, const void* method);
    static void(__thiscall* Orig_check)(Sound_SubGame_Scorer* pThis, int32_t ret, float rate, const void* method);

    // Hooked versions
    float Hooked_getScore(const void* method);
    bool Hooked_isPerfect(const void* method);
    void Hooked_check(int32_t ret, float rate, const void* method);
};




class GameSystem_Item_GlobalData
{
    GameSystem_Item_GlobalData() = default;

private:
    static void __stdcall Hooked_settingDefaultData(void* pThis, const void* method);
    static void(__stdcall* Orig_settingDefaultData)(void* pThis, const void* method);

public:
    static void Hook_GameSystem_Item_GlobalData(uintptr_t dllBase);
    static void* GlobalDataPtr; // holds the latest instance
    static void DumpAllItems_Raw();
    static void DumpOneItem_Raw(uint32_t id);
    static std::map<uint16_t,std::pair<uint32_t,uint32_t>> inventory;
};



class GameSystem_Item_Operator
{
    GameSystem_Item_Operator() = default;

private:
    static void(__stdcall* Orig_ctor)(void* pThis, const void* method);
    static void __stdcall Hooked_ctor(void* pThis, const void* method);

    static void(__stdcall* Orig_subItem)(void* pThis, int32_t itemId, int32_t num, const void* method);
    static void __stdcall Hooked_subItem(void* pThis, int32_t itemId, int32_t num, const void* method);



public:
    static void* ItemOperatorPtr; // Last seen instance pointer
    static uintptr_t GetItemParamAddr;
    static uintptr_t AddItemAddr;

    static void Hook_GameSystem_Item_Operator(uintptr_t dllBase);
    static void* TestDumpItemParam(int32_t itemId);
    static void TestAddItem(int32_t itemId, int32_t num);
    static void ThisTestAddItem(void* ItemOperatorPtr, int32_t itemId, int32_t num);
    static uintptr_t dllBaseSave;

};


class Bases_Item_Slot
{
    Bases_Item_Slot() = default;

private:
    // setItemId hook
    static void(__stdcall* Orig_setItemId)(void* pThis, int32_t itemId, const void* method);
    static void __stdcall Hooked_setItemId(void* pThis, int32_t itemId, const void* method);

    // render hook
    static void(__stdcall* Orig_render)(void* pThis, void* sprite, uint32_t num, const void* method);
    static void __stdcall Hooked_render(void* pThis, void* sprite, uint32_t num, const void* method);


    static void(__stdcall* Orig_SlotSelector_update)(
        void* pThis,
        uint32_t dt,
        bool isSelectEnable,
        const void* method);

    static void __stdcall Hooked_SlotSelector_update(
        void* pThis,
        uint32_t dt,
        bool isSelectEnable,
        const void* method);



public:
    static void* LastSlotPtr; // Last slot seen
    static void Hook_Bases_Item_Slot(uintptr_t dllBase);
    static int16_t getItemNum(int32_t itemId);
    static int32_t  selectedSlot;
};




class Game_Unit_UnitTroop
{
    Game_Unit_UnitTroop() = default;

private:
using FnGetUnitPosX = System::Single_array* (__stdcall*)(
        void* /*this*/,
        bool  /*isTop_orTail*/,
        bool  /*isCheckOtherTop*/,
        bool  /*isLivingOnly*/,
        const void* /*method*/);

    static FnGetUnitPosX Orig_getUnitPosX;

    static System::Single_array* __stdcall Hooked_getUnitPosX(
        void* pThis,
        bool isTop_orTail,
        bool isCheckOtherTop,
        bool isLivingOnly,
        const void* method);

public:

    static void Hook_Game_Unit_UnitTroop(uintptr_t dllBase);
    static float AverageUnitX;
    static uint64_t UnitX_Time;

};


class GameSystem_Effect_Manager
{
    GameSystem_Effect_Manager() = default;

private:
    // P2_GameSystem_Effect_Effect_o* __stdcall (P2_GameSystem_Effect_Manager_o*, int32_t, uint32_t, Vector2*, Vector2*, float, const MethodInfo*)
    static void* (__stdcall* Orig_generateItemEffect)(
        void* pThis,
        int32_t itemId,
        uint32_t effectId,
        void* position,   // UnityEngine_Vector2_o*
        void* velocity,   // UnityEngine_Vector2_o*
        float time,
        const void* method);

    static void* __stdcall Hooked_generateItemEffect(
        void* pThis,
        int32_t itemId,
        uint32_t effectId,
        void* position,   // UnityEngine_Vector2_o*
        void* velocity,   // UnityEngine_Vector2_o*
        float time,
        const void* method);

public:
    static void Hook_GameSystem_Effect_Manager(uintptr_t dllBase);

};