#include <Patapon2Class.h>
#include <Detours/detours.h>
#include <UniversalHookX/src/menu/menu.hpp>


// Statics
Game_Unit_UnitTroop::FnGetUnitPosX Game_Unit_UnitTroop::Orig_getUnitPosX = nullptr;
float Game_Unit_UnitTroop::AverageUnitX=-1.0;
uint64_t Game_Unit_UnitTroop::UnitX_Time=duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

// Hook body
System::Single_array* __stdcall Game_Unit_UnitTroop::Hooked_getUnitPosX(
        void* pThis,
        bool isTop_orTail,
        bool isCheckOtherTop,
        bool isLivingOnly,
        const void* method)
    {
        auto arr = Orig_getUnitPosX(pThis, isTop_orTail, isCheckOtherTop, isLivingOnly, method);
        // AverageUnitX = 0.0f;

            void* troopInfo = *reinterpret_cast<void**>((uint8_t*)pThis+ 0x98);
            int32_t directions = *reinterpret_cast<uint64_t*>((uint8_t*)troopInfo + 0x10);
            int32_t troopType = *reinterpret_cast<uint64_t*>((uint8_t*)troopInfo + 0x14);
            int32_t ctrlType  = *reinterpret_cast<uint64_t*>((uint8_t*)troopInfo + 0x18);

        if (arr && isLivingOnly && directions==0 && troopType==0 &&  ctrlType ==0) 
        {
            uint64_t count = arr->max_length;
            float sum=0;
            if (count > 0 && count < (1ull << 20)) 
            {

                for (uint64_t i = 0; i < count; ++i) 
                {
                    
                    sum +=  arr->m_Items[i];
                }
                AverageUnitX =sum/count;
                UnitX_Time=duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
                // Menu::AddLog("  @ average %04i unit X %4.1f   %i  %i  %i \n",count, AverageUnitX, isTop_orTail, isCheckOtherTop, isLivingOnly);
            } 
        } 

        

        return arr;
    }

void Game_Unit_UnitTroop::Hook_Game_Unit_UnitTroop(uintptr_t dllBase)
{
    


    Orig_getUnitPosX = reinterpret_cast<FnGetUnitPosX>(dllBase + 0x06D3090);
    DetourAttach(reinterpret_cast<PVOID*>(&Orig_getUnitPosX), Hooked_getUnitPosX);


}
