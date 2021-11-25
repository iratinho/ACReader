#include <cstdio>

#include "ACReader.h"

int main(int argc, char* argv[])
{
    ACReader AssaultCubeReader = ACReader(TEXT("ac_client.exe"));
    if(AssaultCubeReader.Init())
    {
        FMemoryData MemoryData;
        while(AssaultCubeReader.Advance(MemoryData))
        {
            const PLocation_float Location = MemoryData.GetData<DataTypes::Location>();
            const PStat_int Health = MemoryData.GetData<DataTypes::Health>();
            const PStat_int Armor = MemoryData.GetData<DataTypes::Armor>();
            const PGun Rifle = MemoryData.GetData<DataTypes::Rifle>();
            const PGun Pistol = MemoryData.GetData<DataTypes::Pistol>();
            const PGrenade Grenade = MemoryData.GetData<DataTypes::Grenade>();

            system("cls");
            printf("Location: x: %f | y: %f | z: %f \n", Location->m_X, Location->m_Y, Location->m_Z);
            printf("Health: %d \n", Health->m_Value);
            printf("Armor: %d \n", Armor->m_Value);
            printf("Rifle: %d / %d \n", Rifle->m_Ammo, Rifle->m_ReservedAmmo);
            printf("Pistol: %d / %d \n", Pistol->m_Ammo, Pistol->m_ReservedAmmo);
            printf("Grenade: %d \n", Grenade->m_Ammo);
        }
    }

    return 0;
}
