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
