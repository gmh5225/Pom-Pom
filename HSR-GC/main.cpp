#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "overlay/device.h"
#include "utils/anti_cheat.h"

static HMODULE h_module = 0;

void Setup() {
  if (!config::Load(h_module)) {
    config::Save();
    Sleep(200);

    if (!config::Load(h_module)) {
      puts("[!] Failed to create config file");
      return;
    }
  }

  uint64_t base_address =
      reinterpret_cast<uint64_t>(GetModuleHandleA("starrailbase.dll"));

  if (utils::GetTextSectionChecksum(base_address) != 0x1434A1A0) {
    globals::china_version = true;

    puts("[>] China game version detected\n[>] If you don't have the Chinese "
         "game version, please create a GitHub issue");
  }

  anti_cheat::Setup(base_address);

  Sleep(15000);

  if (!Direct3D.Initialization())
    puts("[-] Failed to setup Direct3D!");
  else {
    puts("[+] Direct3D setup successfully!");
    printf("[>] Direct3D Present: %p\n[>] Direct3D ResizeBuffers: %p\n",
           Direct3D.Present, Direct3D.ResizeBuffers);
  }
  if (!device::Setup())
    puts("[-] Failed to setup device hooks!");
  else
    puts("[+] Device hooks setup successfully!");

  return overlay::Main();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
    h_module = hModule;

    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Setup, 0, 0, 0);
  }

  return true;
}