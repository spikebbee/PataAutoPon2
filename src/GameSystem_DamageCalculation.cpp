#include <Patapon2Class.h>
#include <Detours/detours.h>
#include <UniversalHookX/src/menu/menu.hpp>



bool(__stdcall* GameSystem_DamageCalculation::Orig_calcAttackPower)(
    void*,
    void*,
    const void*) = nullptr;


bool(__stdcall* GameSystem_DamageCalculation::Orig_calcDamage)(
    void*, void**, void*, void*, void*, float, bool, void*, bool, bool, bool, const void*) = nullptr;


//not used
bool __stdcall GameSystem_DamageCalculation::Hooked_calcAttackPower(
    void* pHitInfo, void* pAttackDamageParam, const void* method)
{
    Menu::AddLog("[DamageCalc] Hook called");

    if (pAttackDamageParam)
    {
        void* param = *(void**)pAttackDamageParam; // dereference once to get param object
        if (param)
        {
            void* attackCalc = *(void**)((uintptr_t)param + 0x18); // attackCalc = param->attackCalc
            if (attackCalc)
            {
                float* minBasePtr = (float*)((uintptr_t)attackCalc + 0x10);
                float* maxBasePtr = (float*)((uintptr_t)attackCalc + 0x14);

                float minBase = *minBasePtr;
                float maxBase = *maxBasePtr;

                float multiplier = Menu::GameHackSetting.DamageMultiplier;
                minBase *= multiplier;
                maxBase *= multiplier;

                *minBasePtr = minBase;
                *maxBasePtr = maxBase;

                // Menu::AddLog("[DamageCalc] minBaseDamage = %.2f, maxBaseDamage = %.2f", minBase, maxBase);
            }
        }
    }

    return GameSystem_DamageCalculation::Orig_calcAttackPower(pHitInfo, pAttackDamageParam, method);
}


bool GameSystem_DamageCalculation::Hooked_calcDamage(
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
    const void* method)
{


    // Menu::AddLog("[calcDamage] accuracyRatio = %.2f, isShield = %d, isEnemy = %d, isGimmick = %d, isMulti = %d\n",
        // accuracyRatio, isShield, isEnemy, isGimmick, isMulti);


    float* hitPower = (float*)((uintptr_t)pHitInfo + 0x18);

    // if attack others
    if(isEnemy==1 || isGimmick==1)
    {
        *hitPower *=Menu::GameHackSetting.DamageMultiplier;
        accuracyRatio = 1.0;
    }

    // if others attack us?
    if(isEnemy==0 && isGimmick==0)
    {
        if(Menu::GameHackSetting.Invincible) return 1;
    }

    bool result = Orig_calcDamage(
        pHitInfo, pHitResult, pDamageParam, pGameStatus, pNbVec,
        accuracyRatio, isShield, pAscActorObj, isEnemy, isGimmick, isMulti, method);


    return result;
}

void GameSystem_DamageCalculation::Hook_GameSystem_DamageCalculation(uintptr_t dllBase)
{
    // Orig_calcAttackPower = reinterpret_cast<decltype(Orig_calcAttackPower)>(dllBase + 0x0548EA0);
    // DetourAttach(&(PVOID&)Orig_calcAttackPower, Hooked_calcAttackPower);

    Orig_calcDamage = reinterpret_cast<decltype(Orig_calcDamage)>(dllBase + 0x05490C0);
    DetourAttach(&(PVOID&)Orig_calcDamage, Hooked_calcDamage);


}