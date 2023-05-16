#pragma once

#include <MinHook/MinHook.h>
#include <iostream>

#include "utils.h"

namespace anti_cheat {
  typedef HANDLE(WINAPI *CREATE_FILE_W)(LPCWSTR, DWORD, DWORD,
                                        LPSECURITY_ATTRIBUTES, DWORD, DWORD,
                                        HANDLE);

  CREATE_FILE_W p_CreateFileW = nullptr;
  CREATE_FILE_W t_CreateFileW;

  HANDLE WINAPI h_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess,
                              DWORD dwShareMode,
                              LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                              DWORD dwCreationDisposition,
                              DWORD dwFlagsAndAttributes,
                              HANDLE hTemplateFile) {
      if (memcmp(lpFileName, L"\\\\.\\ACE-BASE", 24) == 0) {
      wprintf(L"[>] Thread (%i) attempting to communicate with anti-cheat "
              L"driver -> "
              L"%s\n",
              GetCurrentThreadId(), lpFileName);

      SuspendThread(GetCurrentThread()); // 200iq bypass for memory protection
    }

    return p_CreateFileW(lpFileName, dwDesiredAccess, dwShareMode,
                         lpSecurityAttributes, dwCreationDisposition,
                         dwFlagsAndAttributes, hTemplateFile);
  }

  void Setup(uint64_t srbase) {
    if (MH_Initialize() != MH_OK) {
      puts("[-] Failed to initialize MinHook library");

      return;
    }

    if (MH_CreateHookApiEx(
            L"kernelbase", "CreateFileW", &anti_cheat::h_CreateFileW,
            reinterpret_cast<void **>(&p_CreateFileW),
            reinterpret_cast<void **>(&t_CreateFileW)) != MH_OK) {
      puts("[-] Failed to create hook for CreateFileW function");

      return;
    }

    if (MH_EnableHook(t_CreateFileW) != MH_OK) {
      puts("[-] Failed to enable hook for CreateFileW function");

      return;
    }

    utils::Write<uint32_t>(srbase + 0xFCDC0, 0xCCC3C031);
    utils::Write<uint8_t>(srbase + 0xF9940, 0xC3);
    utils::Write<uint16_t>(srbase + 0x1BCBA0, 0xFEEB);
  }

} // namespace anti_cheat