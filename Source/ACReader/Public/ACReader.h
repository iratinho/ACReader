/*
*MIT License

Copyright (c) 2021 iratinho

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once
#include "pch.h"

#ifdef DLL_EXPORT 
   #define ACREADER_API __declspec(dllexport)  
#endif

#ifdef DLL_IMPORT
   #define ACREADER_API __declspec(dllimport)  
#endif

#ifndef ACREADER_API
    #define ACREADER_API
#endif

struct FDataWrapper
{
    // Empty on purpose
    struct None {};

    union Data
    {
        struct Gun
        {
            int m_Ammo;
            int m_ReservedAmmo;    
        };

        struct Grenade
        {
            int m_Ammo;
        };

        template<typename T>
        struct Stats
        {
            T m_Value;
        };

        template <typename T>
        struct Location
        {
            T m_X;
            T m_Y;
            T m_Z;
        };
    };
};

typedef FDataWrapper::Data::Location<float>* PLocation_float;
typedef FDataWrapper::Data::Stats<int>* PStat_int;
typedef FDataWrapper::Data::Gun* PGun;
typedef FDataWrapper::Data::Grenade* PGrenade;

namespace DataTypes
{
    enum Types
    {
        None,
        Rifle,
        Pistol,
        Grenade,
        Health,
        Armor,
        Location,
        END_PlayerEntity
    };
}

namespace Internal
{
    class FAnyEntityData
    {
    public:
        template <typename RetType>
        FORCEINLINE RetType* GetDataPtr_Internal(RetType (FAnyEntityData::* m_Member)) { return &(this->*m_Member); }

        FDataWrapper::Data::Location<float> m_Location;
        FDataWrapper::Data::Stats<int> m_Armor;
        FDataWrapper::Data::Stats<int> m_Health; 
        FDataWrapper::Data::Gun m_Rifle;
        FDataWrapper::Data::Gun m_Pistol;
        FDataWrapper::Data::Grenade m_Grenade;
    };
}

namespace Traits
{
    template <DataTypes::Types> struct TConvertDataType { typedef FDataWrapper::None* Type; };

    template <> struct TConvertDataType<DataTypes::Location> { typedef FDataWrapper::Data::Location<float> Type; inline static auto Ptr = &Internal::FAnyEntityData::m_Location; };
    template <> struct TConvertDataType<DataTypes::Health>   { typedef FDataWrapper::Data::Stats<int> Type;      inline static auto Ptr = &Internal::FAnyEntityData::m_Health; };
    template <> struct TConvertDataType<DataTypes::Armor>    { typedef FDataWrapper::Data::Stats<int> Type;      inline static auto Ptr = &Internal::FAnyEntityData::m_Armor; };
    template <> struct TConvertDataType<DataTypes::Rifle>    { typedef FDataWrapper::Data::Gun Type;             inline static auto Ptr = &Internal::FAnyEntityData::m_Rifle;};
    template <> struct TConvertDataType<DataTypes::Pistol>   { typedef FDataWrapper::Data::Gun Type;             inline static auto Ptr = &Internal::FAnyEntityData::m_Pistol; };
    template <> struct TConvertDataType<DataTypes::Grenade>  { typedef FDataWrapper::Data::Grenade Type;         inline static auto Ptr = &Internal::FAnyEntityData::m_Grenade; };
}

class ACREADER_API FPlayerEntity : Internal::FAnyEntityData
{
public:
    FPlayerEntity() = default;
    
    FPlayerEntity(LPVOID DataAddress);

    // This can be moved to a more generic place when we have other types of entities
    template <typename Type>
    auto GetData() { return GetDataPtr_Internal<typename Type::Type>(Type::Ptr); }

    bool RefreshData(HANDLE m_ProcessHandle);

private:
    LPVOID m_DataAddress;

    int PosXOffset = 0x0004;
    int PosYOffset = 0x00008;
    int PosZOffset = 0x000c;
    int HealthOffset = 0x00f8;
    int ArmorOffset = 0x00fc;
    int PistolReserveAmmoOffset = 0x0114;
    int RifleReserveAmmoOffset = 0x128;
    int PistolAmmoOffset = 0x013c;
    int RifleAmmoOffset = 0x150;
    int GrenadeAmmoOffset = 0x0158;
};

struct ACREADER_API FMemoryData
{
    template <DataTypes::Types Type>
    auto GetData()
    {
        if(Type < DataTypes::END_PlayerEntity)
            return PlayerEntity.GetData<Traits::TConvertDataType<Type>>();
    }

private:
    FPlayerEntity PlayerEntity;

    friend class ACReader;
};

class ACREADER_API ACReader
{
public:
    ACReader(const TCHAR* ProcessName);

    bool Init();

    // Do one more cycle and fetch all data from memory
    bool Advance(FMemoryData& EntityWrapper) const;

private:
    const TCHAR* m_ProcessName;
    HANDLE m_ProcessHandle;
    BYTE* m_ModuleBaseAddress;
    LPVOID m_PlayerEntityAddress;
};


