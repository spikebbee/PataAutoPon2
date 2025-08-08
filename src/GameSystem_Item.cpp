#include <Patapon2Class.h>
#include <types.h>
#include <Detours/detours.h>
#include <UniversalHookX/src/menu/menu.hpp>



void* GameSystem_Item_GlobalData::GlobalDataPtr = nullptr;
std::map<uint16_t,std::pair<uint32_t,uint32_t>> GameSystem_Item_GlobalData::inventory;

void(__stdcall* GameSystem_Item_GlobalData::Orig_settingDefaultData)(
    void* pThis,
    const void* method) = nullptr;

void __stdcall GameSystem_Item_GlobalData::Hooked_settingDefaultData(void* pThis, const void* method)
{
    GlobalDataPtr = pThis;
    // Menu::AddLog("[ItemGlobalData] settingDefaultData called, ptr = 0x%p\n", pThis);
    // Call original
    Orig_settingDefaultData(pThis, method);
}


void GameSystem_Item_GlobalData::DumpOneItem_Raw(uint32_t id)
{
    if (!GameSystem_Item_GlobalData::GlobalDataPtr)
    {
        Menu::AddLog("[ItemDump] GlobalDataPtr is null.\n");
        return;
    }

    void* globalData = GameSystem_Item_GlobalData::GlobalDataPtr;

    // Offset 0x00: pointer to items array
    void* itemsArray = *reinterpret_cast<void**>((uint8_t*)globalData + 0x18);
    if (!itemsArray)
    {
        Menu::AddLog("[ItemDump] itemsArray is null.\n");
        return;
    }

    uint64_t maxLength = *reinterpret_cast<uint64_t*>((uint8_t*)itemsArray + 0x18);
    void** itemPtrArray = reinterpret_cast<void**>((uint8_t*)itemsArray + 0x20);

    if(id >= maxLength) return;
    
    void* item = itemPtrArray[id];
    if (!item) return;

    // Offset 0x10: int16_t num
    int16_t num = *reinterpret_cast<int16_t*>((uint8_t*)item + 0x10);
    auto itemParam = GameSystem_Item_Operator::TestDumpItemParam(id);
    if(!itemParam) return;
    int16_t categoryId = *reinterpret_cast<int16_t*>((uintptr_t)itemParam + 0x1C); 
    auto rawStr = *(System::String**)((uintptr_t)itemParam + 0x28);//icon model name
    wchar_t* wideStr = rawStr->buffer;
    int length = rawStr->size;
    char utf8Str[1024];
    int bytesWritten = WideCharToMultiByte( CP_UTF8, 0, wideStr, length, utf8Str, sizeof(utf8Str) - 1, NULL, NULL);
    utf8Str[bytesWritten] = '\0';
    if (length>0)
    {
        Menu::AddLog(" - ID: %llu, Num: %d,  category: %i,modelname: %s\n", id, num, categoryId, utf8Str);
        GameSystem_Item_Operator::TestDumpItemParam(id);
    }
    
}


void GameSystem_Item_GlobalData::DumpAllItems_Raw()
{
    if (!GameSystem_Item_GlobalData::GlobalDataPtr)
    {
        Menu::AddLog("[ItemDump] GlobalDataPtr is null.\n");
        return;
    }

    void* globalData = GameSystem_Item_GlobalData::GlobalDataPtr;

    // Offset 0x00: pointer to items array
    void* itemsArray = *reinterpret_cast<void**>((uint8_t*)globalData + 0x18);
    if (!itemsArray)
    {
        Menu::AddLog("[ItemDump] itemsArray is null.\n");
        return;
    }

    // Offset 0x18: max_length
    uint64_t maxLength = *reinterpret_cast<uint64_t*>((uint8_t*)itemsArray + 0x18);
    // Offset 0x20: pointer array of item pointers
    void** itemPtrArray = reinterpret_cast<void**>((uint8_t*)itemsArray + 0x20);

    Menu::AddLog("[ItemDump] Dumping %llu items:\n", maxLength);
    if(maxLength>1000) return;
    for (uint32_t id = 0; id < maxLength; ++id)
    {
        DumpOneItem_Raw(id);
    }
}


void GameSystem_Item_GlobalData::Hook_GameSystem_Item_GlobalData(uintptr_t dllBase)
{
    Orig_settingDefaultData = reinterpret_cast<decltype(Orig_settingDefaultData)>(dllBase + 0x05AA060);
    auto settingDefaultData = &GameSystem_Item_GlobalData::Hooked_settingDefaultData;
    DetourAttach(&reinterpret_cast<PVOID&>(Orig_settingDefaultData), reinterpret_cast<PVOID&>(settingDefaultData));
}



//---------------------------GameSystem_Item_Operator---------------------------
void* GameSystem_Item_Operator::ItemOperatorPtr = nullptr;
void(__stdcall* GameSystem_Item_Operator::Orig_ctor)(void*, const void*) = nullptr;
void(__stdcall* GameSystem_Item_Operator::Orig_subItem)(void* pThis, int32_t itemId, int32_t num, const void* method);

uintptr_t GameSystem_Item_Operator::GetItemParamAddr;
uintptr_t GameSystem_Item_Operator::AddItemAddr;
uintptr_t GameSystem_Item_Operator::dllBaseSave;
using FnGetItemParam = void* (__stdcall*)(void* pThis, int32_t itemId, const void* method);
using FnAddItem = void* (__stdcall*)(void* pThis, int32_t itemId, int32_t num, const void* method);
using FnAllocObject = void* (__fastcall*)(void* typeInfo);




void* GameSystem_Item_Operator::TestDumpItemParam(int32_t itemId)
{
    if (!GameSystem_Item_Operator::ItemOperatorPtr)
        return nullptr;

    auto GetItemParam = reinterpret_cast<FnGetItemParam>(GameSystem_Item_Operator::GetItemParamAddr);

    void* itemParam = GetItemParam(GameSystem_Item_Operator::ItemOperatorPtr, itemId, nullptr);
    if (!itemParam)
    {
        Menu::AddLog("[ItemParam] itemId %d not found (null)\n", itemId);
        return nullptr;
    }

    return itemParam;
}


void GameSystem_Item_Operator::TestAddItem(int32_t itemId, int32_t num)
{

    auto AddItem = reinterpret_cast<FnAddItem>(GameSystem_Item_Operator::AddItemAddr);
    static uint8_t dummyBuffer[0x1024]; //buffer
    auto* instance = reinterpret_cast<void*>(dummyBuffer);
    using FnCtor = void(__stdcall*)(void*, const void*);
    FnCtor Ctor_ItemOperator = reinterpret_cast<FnCtor>(dllBaseSave + 0x05AAC30);
    Ctor_ItemOperator(instance, nullptr); // now it's fully initialized!
    Menu::AddLog("  ! try add item_%i by %i\n",itemId, num);
    AddItem(instance, itemId, num, nullptr);
}

void __stdcall GameSystem_Item_Operator::Hooked_subItem(void* pThis, int32_t itemId, int32_t num, const void* method)
{
    // Menu::AddLog("[ItemOperator] subItem called: itemId=%d, num=%d\n", itemId, num);

    auto oldnum = Bases_Item_Slot::getItemNum(itemId);
    if((oldnum-num)<1 && Menu::GameHackSetting.NoZeroItem) return;

    Orig_subItem(pThis, itemId, num, method);
}



void __stdcall GameSystem_Item_Operator::Hooked_ctor(void* pThis, const void* method)
{
    ItemOperatorPtr = pThis;
    // Menu::AddLog("[ItemOperator] ctor called, this = %p\n", pThis);
    // Call original constructor
    Orig_ctor(pThis, method);
}

void GameSystem_Item_Operator::Hook_GameSystem_Item_Operator(uintptr_t dllBase)
{
    Orig_ctor = reinterpret_cast<decltype(Orig_ctor)>(dllBase + 0x05AAC30);
    DetourAttach(&(PVOID&)Orig_ctor, Hooked_ctor);

    Orig_subItem = reinterpret_cast<decltype(Orig_subItem)>(dllBase + 0x05AAF60);
    DetourAttach(&(PVOID&)Orig_subItem, Hooked_subItem);

    GetItemParamAddr = dllBase + 0x05AB0B0;
    AddItemAddr = dllBase + 0x05AAE30;

    dllBaseSave = dllBase;

}



//---------------------Bases_Item_Slot---------------------------------



void* Bases_Item_Slot::LastSlotPtr = nullptr;
decltype(Bases_Item_Slot::Orig_setItemId) Bases_Item_Slot::Orig_setItemId = nullptr;
void(__stdcall* Bases_Item_Slot::Orig_render)(void*, void*, uint32_t, const void*) = nullptr;

int32_t Bases_Item_Slot:: selectedSlot = -1;


int16_t Bases_Item_Slot::getItemNum(int32_t itemId)
{
    if (!GameSystem_Item_GlobalData::GlobalDataPtr) return 0;
    void* globalData = GameSystem_Item_GlobalData::GlobalDataPtr;
    void* itemsArray = *reinterpret_cast<void**>((uint8_t*)globalData + 0x18);
    if (!itemsArray) return 0;
    uint64_t maxLength = *reinterpret_cast<uint64_t*>((uint8_t*)itemsArray + 0x18);
    void** itemPtrArray = reinterpret_cast<void**>((uint8_t*)itemsArray + 0x20);
    if(itemId >= maxLength) return 0;
    void* item = itemPtrArray[itemId];
    if (!item) return 0 ;
    int16_t num = *reinterpret_cast<int16_t*>((uint8_t*)item + 0x10);

    return num;
}


void __stdcall Bases_Item_Slot::Hooked_setItemId(void* pThis, int32_t itemId, const void* method)
{
    LastSlotPtr = pThis;
    Orig_setItemId(pThis, itemId, method);


    uint16_t slotId = *reinterpret_cast<uint16_t*>((uintptr_t)LastSlotPtr + 0x20); 
    auto num = getItemNum(itemId);


    GameSystem_Item_GlobalData::inventory[slotId] = {itemId,num};

    // Menu::AddLog("[Bases_Item_Slot]  itemId = %d, slotId  = %d, num  = %d\n", itemId, slotId, num );

    // GameSystem_Item_GlobalData::DumpOneItem_Raw(itemId);
    // GameSystem_Item_Operator::TestAddItem(itemId,99);
}


void __stdcall Bases_Item_Slot::Hooked_render(void* pThis, void* sprite, uint32_t num, const void* method)
{
    Orig_render(pThis, sprite, num, method);

    uint16_t slotId = *reinterpret_cast<uint16_t*>((uintptr_t)pThis + 0x20); 
    int32_t itemId =   *reinterpret_cast<int32_t*>((uintptr_t)pThis + 0x38); 

    // Menu::AddLog("[Slot] render called. %i num = %i\n",slotId, num);


    GameSystem_Item_GlobalData::inventory[slotId] = {itemId,num};

    if(Menu::GameHackSetting.NoZeroItem && num==0)
    {
        GameSystem_Item_Operator::TestAddItem(itemId, 1);
    } 


}


void(__stdcall* Bases_Item_Slot::Orig_SlotSelector_update)(
    void*,
    uint32_t,
    bool,
    const void*) = nullptr;


void __stdcall Bases_Item_Slot::Hooked_SlotSelector_update(
    void* pThis,
    uint32_t dt,
    bool isSelectEnable,
    const void* method)
{

    Orig_SlotSelector_update(pThis, dt, isSelectEnable, method);

    auto selected = *reinterpret_cast<int32_t*>((uintptr_t)pThis + 0x2C); 

    Bases_Item_Slot:: selectedSlot = selected;



    // Menu::AddLog("SlotSelector_update called. %i \n",selected);

}

void Bases_Item_Slot::Hook_Bases_Item_Slot(uintptr_t dllBase)
{
    Orig_setItemId = reinterpret_cast<decltype(Orig_setItemId)>(dllBase + 0x087E430);
    DetourAttach(&(PVOID&)Orig_setItemId, Hooked_setItemId);

    Orig_render = reinterpret_cast<decltype(Orig_render)>(dllBase + 0x087E580);
    DetourAttach(&(PVOID&)Orig_render, Hooked_render);

    Orig_SlotSelector_update = reinterpret_cast<decltype(Orig_SlotSelector_update)>(dllBase + 0x087E9F0);
    DetourAttach(&(PVOID&)Orig_SlotSelector_update, Hooked_SlotSelector_update);

}