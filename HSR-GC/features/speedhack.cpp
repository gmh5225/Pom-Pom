#include "features.h"

namespace features::speedhack {
  void UpdateSpeed(const float speed, uint64_t game_assembly,
                   uint64_t unity_player, const bool is_battle) {
    if (is_battle) {
      utils::Write<float>(
          utils::Read<__int64>(utils::Read<__int64>(game_assembly + 0x8CAA6A0) +
                               0xC0) +
              0x1DC,
          speed);
      utils::Write<float>(utils::Read<__int64>(unity_player + 0x1D21D78) + 0xFC,
                          1.f);
    } else {
      utils::Write<float>(
          utils::Read<__int64>(utils::Read<__int64>(game_assembly + 0x8CAA6A0) +
                               0xC0) +
              0x1DC,
          1.f);
      utils::Write<float>(utils::Read<__int64>(unity_player + 0x1D21D78) + 0xFC,
                          speed);
    }
  }

  void UpdateWorld(uint64_t game_assembly, uint64_t unity_player) {
    if (hooks::game::phase != 12) {
      return;
    }

    float speed = 1.f;

    if (globals::world::speed_hack) {
      speed = hooks::game::get_is_in_dialog() || GetAsyncKeyState(VK_CAPITAL)
                  ? globals::world::dialogue_speed
                  : globals::world::global_speed;
    }

    UpdateSpeed(speed, game_assembly, unity_player, false);

    utils::Write<uint8_t>(game_assembly + 0x51292C0,
                          globals::world::peeking ? 0xC3 : 0x40);
    utils::Write<uint8_t>(game_assembly + 0x5800F40,
                          globals::world::invisibility ? 0xC3 : 0x40);
  }

  void UpdateBattle(uint64_t game_assembly, uint64_t unity_player) {
    if (hooks::game::phase != 15) {
      return;
    }

    UpdateSpeed(globals::battle::speed_hack ? globals::battle::battle_speed
                                            : 1.f,
                game_assembly, unity_player, true);

    utils::Write<uint32_t>(game_assembly + 0x5DA5F20,
                           globals::battle::auto_battle_unlock ? 0x90C3C031
                                                               : 0x83485340);
  }
} // namespace features::speedhack