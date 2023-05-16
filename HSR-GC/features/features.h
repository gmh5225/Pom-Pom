#pragma once

#include "../globals.hpp"
#include "../utils/config.h"
#include "../utils/hooks.h"
#include "../utils/utils.h"

namespace features {
  namespace speedhack {
    void UpdateSpeed(const float speed, uint64_t game_assembly,
                     uint64_t unity_player, const bool is_battle);

    void UpdateWorld(uint64_t game_assembly, uint64_t unity_player);

    void UpdateBattle(uint64_t game_assembly, uint64_t unity_player);
  } // namespace speedhack

  namespace dialogue {
    void AutoDialogue();
  }

  namespace other {
    void Update(uint64_t unity_player);
  }
  namespace visuals {
      void Profile(uint64_t game_assembly);
  }
} // namespace features