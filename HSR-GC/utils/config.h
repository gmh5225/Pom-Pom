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

    bool Profile = false;
    bool Uid = false;
    bool WorldLevel = false;
    bool Xp = false;
    bool PlayerLevel = false;
    int Uid_v = 0;
    int WorldLevel_v = 6;
    int Xp_v = 0;
    int PlayerLevel_v = 70;
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
                                     Fps,Profile,Uid_v ,WorldLevel_v , Xp_v , PlayerLevel_v , Uid , WorldLevel , Xp , PlayerLevel )

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

    globals::visuals::profile = j.get<Config>().Profile;
    globals::visuals::uidvalue = j.get<Config>().Uid_v;
    globals::visuals::worldlevelvalue = j.get<Config>().WorldLevel_v;
    globals::visuals::xpvalue = j.get<Config>().Xp_v;
    globals::visuals::playerlevelvalue = j.get<Config>().PlayerLevel_v;
    globals::visuals::uid = j.get<Config>().Uid;
    globals::visuals::worldlevel = j.get<Config>().WorldLevel;
    globals::visuals::xp = j.get<Config>().Xp;
    globals::visuals::playerlevel = j.get<Config>().PlayerLevel;
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

          globals::other::fps_unlock,          globals::other::fps,
          globals::visuals::profile, globals::visuals::uid,
      globals::visuals::worldlevel , globals::visuals::xp , globals::visuals::playerlevel ,  
      globals::visuals::uidvalue , globals::visuals::worldlevelvalue ,globals::visuals::xpvalue ,
      globals::visuals::playerlevelvalue };

    std::ofstream o(file_path);
    o << std::setw(4) << j << std::endl;
  }
} // namespace config
