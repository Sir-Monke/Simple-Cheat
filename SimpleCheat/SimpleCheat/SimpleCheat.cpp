#include "stdafx.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"
#include "mem.h"

int main()
{
    const int newHealth = 5000;
    const int newArmor = 2100;
    const int newAmmoAll = 1000;
    const int newGrenade = 10;

    uintptr_t moduleBase = 0, localPlayer = 0, healthAddr = 0, ArmorAddr = 0, Ammo = 0;
    bool health = false, armor = false, ammo = false;

    HANDLE hProcess = 0;
    DWORD procId = GetProcId(L"ac_client.exe");

    if (procId)
    {
        std::cout << procId;
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
        uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");
        localPlayer = moduleBase + 0x10f4f4;
        healthAddr = FindDMAAddy(hProcess, localPlayer, { 0xF8 });
        ArmorAddr = FindDMAAddy(hProcess, localPlayer, { 0xFC });
        Ammo = FindDMAAddy(hProcess, localPlayer, { 0x148 });
    }
    else
    {
        std::cout << "You idiot you didn't start ze fukin game";
        return 0;
    }

    DWORD dwExit = 0;
    while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
    {
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            health = !health;
            std::cout << "done Health\n";
        }
        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            ammo = !ammo;
            std::cout << "done Ammo\n";
        }
        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            armor = !armor;
            std::cout << "done Armor\n";
        }
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            return 0;
        }
        if (health)
        {
            mem::PatchEx((BYTE*)healthAddr, (BYTE*)&newHealth, sizeof(newHealth), hProcess);
        }
        if (ammo)
        {
            mem::PatchEx((BYTE*)Ammo, (BYTE*)&newAmmoAll, sizeof(newAmmoAll), hProcess);
        }
        if (armor)
        {
            mem::PatchEx((BYTE*)ArmorAddr, (BYTE*)&newAmmoAll, sizeof(newAmmoAll), hProcess);
        }

        Sleep(10);
    }

    std::cout << "Process not found, press enter to exit\n";
    return 0;
}