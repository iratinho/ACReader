#pragma once
#include "pch.h"

namespace MemoryUtils
{
    inline DWORD FindProcessID(const wchar_t* ProcessName)
    {
        PROCESSENTRY32 ProcessEntry;
        ZeroMemory(&ProcessEntry, sizeof ProcessEntry);
        ProcessEntry.dwSize = sizeof ProcessEntry;

        // Traverse process's to find the process id for our target process name
        const HANDLE ProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (Process32First(ProcessSnapshot, &ProcessEntry))
        {
            do
            {
                if (!wcscmp(ProcessEntry.szExeFile, ProcessName))
                {
                    CloseHandle(ProcessSnapshot);
                    return ProcessEntry.th32ProcessID;
                }
            }             while (Process32Next(ProcessSnapshot, &ProcessEntry));
        }

        return {};
    }

    inline BYTE* FindModuleBaseAddress(DWORD ProcessID, const wchar_t* ModuleName)
    {
        MODULEENTRY32 ModuleEntry;
        ZeroMemory(&ModuleEntry, sizeof ModuleEntry);
        ModuleEntry.dwSize = sizeof ModuleEntry;

        // Traverse modules in this process to find our target module
        const HANDLE ModuleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessID);
        if (Module32First(ModuleSnapshot, &ModuleEntry))
        {
            do
            {
                if (!wcscmp(ModuleEntry.szModule, ModuleName))
                {
                    CloseHandle(ModuleSnapshot);
                    return ModuleEntry.modBaseAddr;
                }
            }while (Module32Next(ModuleSnapshot, &ModuleEntry));
        }

        return {};
    }
}
