#pragma once

#include <MinHook/MinHook.h>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "../globals.hpp"
#include "utils.h"

namespace hooks {
  static __int64(__fastcall *o_setcurrentphase)(__int64 a1, int a2, __int64 a3,
                                                char a4) = nullptr;

  static char(__fastcall *o_get_isindialog)(__int64 a1) = nullptr;

  static __int64(__fastcall *o_isautobattle)(__int64 a1);

  static char(__fastcall *o_setautobattleflag)(__int64 a1, unsigned __int8 a2);

  struct HookData {
    std::string name;
    DWORD rva;

    void *hook;
    void *original;
  };

  namespace game {
    inline int phase = 0;

    inline std::chrono::steady_clock::time_point last_call_time;

    /* because get_isindialog returns 0 in some dialogs (HSR developers, please
     * fix this) */
    inline bool get_is_in_dialog() {
      auto current_time = std::chrono::steady_clock::now();
      auto time_since_last_call =
          std::chrono::duration_cast<std::chrono::milliseconds>(current_time -
                                                                last_call_time);
      return time_since_last_call.count() < 25;
    }

    inline const char *get_phase_in_text() {
      const char *phaseText = "Indefinite";

      phase == 12 ? phaseText = "World" : 0;
      phase == 15 ? phaseText = "Battle" : 0;
      hooks::game::get_is_in_dialog() ? phaseText = "Dialogue" : 0;

      return phaseText;
    }

  } // namespace game

  inline char __fastcall h_setautobattleflag(__int64 a1, unsigned __int8 a2) {
    return o_setautobattleflag(a1, a2);
  }

  inline __int64 __fastcall h_isautobattle(__int64 a1) {
    auto ret = o_isautobattle(a1);

    if (!ret && globals::battle::force_battle) {
      h_setautobattleflag(a1, 1);
    }

    return ret;
  }

  /* (RPG.Client.GamePhaseManager.SetCurrentPhase) */
  inline __int64 __fastcall h_setcurrentphase(__int64 a1, int a2, __int64 a3,
                                              char a4) {
    game::phase = a2;
    return o_setcurrentphase(a1, a2, a3, a4);
  }

  /* (RPG.Client.DialogueManager.get_IsInDialog) */
  inline char __fastcall h_get_isindialog(__int64 a1) {
    // printf("called->get_isindialog()\n");
    game::last_call_time = std::chrono::steady_clock::now();
    return o_get_isindialog(a1);
  }

  inline void Setup() {
    std::vector<HookData> v_hooks = {};

    if (globals::china_version) {
      v_hooks.push_back({"setcurrentphase", 0x5B9DFD0, &h_setcurrentphase,
                         &o_setcurrentphase});
      v_hooks.push_back(
          {"get_isindialog", 0x5ADD450, &h_get_isindialog, &o_get_isindialog});
      v_hooks.push_back(
          {"isautobattle", 0x512F580, &h_isautobattle, &o_isautobattle});
      v_hooks.push_back({"setautobattleflag", 0x5130040, &h_setautobattleflag,
                         &o_setautobattleflag});
    } else {
      v_hooks.push_back({"setcurrentphase", 0x5B9E130, &h_setcurrentphase,
                         &o_setcurrentphase});
      v_hooks.push_back(
          {"get_isindialog", 0x5ADD460, &h_get_isindialog, &o_get_isindialog});
      v_hooks.push_back(
          {"isautobattle", 0x512F580, &h_isautobattle, &o_isautobattle});
      v_hooks.push_back({"setautobattleflag", 0x5130040, &h_setautobattleflag,
                         &o_setautobattleflag});
    }

    static auto CreateHook = [](LPVOID pTarget, LPVOID pDetour,
                                LPVOID *ppOriginal) -> bool {
      return MH_CreateHook(pTarget, pDetour, ppOriginal) == MH_OK &&
             MH_EnableHook(pTarget) == MH_OK;
    };

    uint64_t game_assembly = 0;

    while (!game_assembly)
      game_assembly =
          reinterpret_cast<uint64_t>(GetModuleHandleA("gameassembly.dll"));

    for (auto &hook : v_hooks) {
      if (!CreateHook(reinterpret_cast<void **>(game_assembly + hook.rva),
                      hook.hook, reinterpret_cast<void **>(hook.original))) {
        printf("[-] Failed to create/enable hook for %s\n", hook.name.c_str());
      }
    }
  }
} // namespace hooks