#pragma once

#include <Json/json.hpp>
#include <fstream>
#include <string>

#include "../globals.hpp"

using json = nlohmann::json;

namespace config {
  struct Config {
    bool EnableWorldSpeedHack = false;
    float GlobalSpeed = 1.f;
    float DialogueSpeed = 1.f;

    bool DumpEnemies = false;
    bool Peeking = false;
    bool AutoDialogue = false;
    bool MouseMode = false;
    bool Invisibility = false;

    bool EnableBattleSpeedHack = false;
    float BattleSpeed = 1.f;
    bool AutoBattleUnlock = false;
    bool ForceBattle = false;

    bool FpsUnlock = false;
    int Fps = 60;
  };

  inline std::string GetModulePath(HMODULE hModule) {
    char path[MAX_PATH] = {};
    GetModuleFileNameA(hModule, path, MAX_PATH);

    return std::filesystem::path(path).parent_path().string();
  }

  inline std::filesystem::path file_path;

  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Config, EnableWorldSpeedHack, GlobalSpeed,
                                     DialogueSpeed, DumpEnemies, Peeking,
                                     AutoDialogue, MouseMode, Invisibility,
                                     EnableBattleSpeedHack, BattleSpeed,
                                     AutoBattleUnlock, ForceBattle, FpsUnlock,
                                     Fps)

  static bool Load(HMODULE hModule) {
    json j;

    file_path = GetModulePath(hModule) + "\\config.json";
    std::ifstream f(file_path, std::ifstream::binary);
    if (f.fail())
      return false;

    f >> j;

    globals::world::speed_hack = j.get<Config>().EnableWorldSpeedHack;
    globals::world::global_speed = j.get<Config>().GlobalSpeed;
    globals::world::dialogue_speed = j.get<Config>().DialogueSpeed;

    globals::world::dump_enemys = j.get<Config>().DumpEnemies;
    globals::world::peeking = j.get<Config>().Peeking;
    globals::world::auto_dialogue = j.get<Config>().AutoDialogue;
    globals::world::mouse_mode = j.get<Config>().MouseMode;
    globals::world::invisibility = j.get<Config>().Invisibility;

    globals::battle::speed_hack = j.get<Config>().EnableBattleSpeedHack;
    globals::battle::battle_speed = j.get<Config>().BattleSpeed;
    globals::battle::auto_battle_unlock = j.get<Config>().AutoBattleUnlock;
    globals::battle::force_battle = j.get<Config>().ForceBattle;

    globals::other::fps_unlock = j.get<Config>().FpsUnlock;
    globals::other::fps = j.get<Config>().Fps;

    return true;
  }

  static void Save() {
    const json j = Config{
        globals::world::speed_hack,          globals::world::global_speed,
        globals::world::dialogue_speed,

        globals::world::dump_enemys,         globals::world::peeking,
        globals::world::auto_dialogue,       globals::world::mouse_mode,
        globals::world::invisibility,

        globals::battle::speed_hack,         globals::battle::battle_speed,
        globals::battle::auto_battle_unlock, globals::battle::force_battle,

        globals::other::fps_unlock,          globals::other::fps};

    std::ofstream o(file_path);
    o << std::setw(4) << j << std::endl;
  }
} // namespace config
