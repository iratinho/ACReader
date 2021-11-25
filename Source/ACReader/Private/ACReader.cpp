#include "ACReader.h"
#include <iostream>

namespace EntityOffsets
{
    int PlayerEntity = 0x109B74;
}

FPlayerEntity::FPlayerEntity(LPVOID DataAddress)
    : m_DataAddress(DataAddress)
{
}

bool FPlayerEntity::RefreshData(HANDLE m_ProcessHandle)
{
    BYTE DataBucket[1024*2];
    if(!ReadProcessMemory(m_ProcessHandle, m_DataAddress ,&DataBucket,sizeof DataBucket,nullptr))
    {
        std::cerr << GetLastError() << std::endl;
        return false;
    }
        
    m_Location.m_X = DataBucket[PosXOffset];
    m_Location.m_Y = DataBucket[PosYOffset];
    m_Location.m_Z = DataBucket[PosZOffset];
    m_Health.m_Value = DataBucket[HealthOffset];
    m_Armor.m_Value = DataBucket[ArmorOffset];
    m_Rifle.m_Ammo = DataBucket[RifleAmmoOffset];
    m_Rifle.m_ReservedAmmo = DataBucket[RifleReserveAmmoOffset];
    m_Pistol.m_Ammo = DataBucket[PistolAmmoOffset];
    m_Pistol.m_ReservedAmmo = DataBucket[PistolReserveAmmoOffset];
    m_Grenade.m_Ammo = DataBucket[GrenadeAmmoOffset];

    return true;
}

ACReader::ACReader(const TCHAR* ProcessName)
    : m_ProcessName(ProcessName)
    , m_ProcessHandle(nullptr)
    , m_ModuleBaseAddress(nullptr)
    , m_PlayerEntityAddress(nullptr)
{
}

bool ACReader::Init()
{
    const DWORD TargetProcessID = MemoryUtils::FindProcessID(m_ProcessName);

    m_ModuleBaseAddress = MemoryUtils::FindModuleBaseAddress(TargetProcessID, m_ProcessName);

    if (m_ModuleBaseAddress == nullptr)
    {
        std::cerr << "[Error]: Unable to find the base module address. (Error: " << GetLastError() << ")" << std::endl;
        return false;
    }
    
    m_ProcessHandle = OpenProcess(PROCESS_VM_READ, false, TargetProcessID);

    if (m_ProcessHandle == nullptr)
    {
        std::cerr << "[Error]: Unable to open process. (Error: " << GetLastError() << ")" << std::endl;
        return false;
    }
    
    // With the PlayerEntity offset lets grab the pointer that points to the PlayerEntity
    const BOOL bSuccess = ReadProcessMemory(m_ProcessHandle, m_ModuleBaseAddress + EntityOffsets::PlayerEntity,&m_PlayerEntityAddress, sizeof m_PlayerEntityAddress, nullptr);

    if(!bSuccess)
    {
        std::cerr << "[Error]: Unable to find the PlayerEntity pointer while reading the external process memory. (Error: " << GetLastError() << ")" << std::endl;
        return false;
    }
    
    return true;
}

// TODO: Add offset parameter so that we allow client code to fetch more players instead of the first one
bool ACReader::Advance(FMemoryData& EntityWrapper) const
{
	// TODO need to avoid creating a FPlayerEntity all the time, we can just update the internal data pointer
    EntityWrapper.PlayerEntity = FPlayerEntity(m_PlayerEntityAddress);
    return EntityWrapper.PlayerEntity.RefreshData(m_ProcessHandle);
}
