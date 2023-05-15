#include "features.h"

namespace features::other {
  void Update(uint64_t unity_player) {
    if (globals::other::fps_unlock) {
      if (globals::other::fps > 59) {
        utils::Write<uint32_t>(unity_player + 0x1C4E000, globals::other::fps);
      }
    } else {
      utils::Write<uint32_t>(unity_player + 0x1C4E000, 60);
    }
  }
} // namespace features::other