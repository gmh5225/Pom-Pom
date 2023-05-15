#pragma once

#include <Windows.h>

namespace globals {
  inline bool show_menu = true;
  inline bool unload = false;
  inline bool china_version = false;

  namespace world {
    inline bool speed_hack = false;
    inline float global_speed = 1.f;
    inline float dialogue_speed = 1.f;

    inline bool dump_enemys = false;
    inline bool peeking = false;
    inline bool auto_dialogue = false;
    inline bool mouse_mode = false;
    inline bool invisibility = false;
  } // namespace world

  namespace battle {
    inline bool speed_hack = false;
    inline float battle_speed = 1.f;

    inline bool auto_battle_unlock = false;
    inline bool force_battle = false;
  } // namespace battle

  namespace other {
    inline bool fps_unlock = false;
    inline int fps = 60;
  } // namespace other

  namespace hotkeys {
    inline int world_speed = VK_F2;
    inline int battle_speed = VK_F3;
    inline int auto_dialogue = VK_F4;
    inline int invisibility = VK_F5;
    inline int force_auto_battle = VK_F6;
    inline int peeking = VK_F7;
  } // namespace hotkeys
} // namespace globals