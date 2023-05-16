#include "features.h"

namespace features::visuals {
    void Profile(uint64_t game_assembly) {
        uintptr_t profileAdress = utils::Read<__int64>(
            utils::Read<__int64>(
                utils::Read<__int64>(
                    utils::Read<__int64>(
                        utils::Read<__int64>(game_assembly + 0x8CAA6A0) + 0xC0) + 0x110) + 0x20) + 0x10);

        if (globals::visuals::profile && globals::visuals::uid) {
            utils::Write<uint64_t>(
                 profileAdress + 0x20,
                globals::visuals::uidvalue);
        }
        if (globals::visuals::profile && globals::visuals::worldlevel) {
            utils::Write<uint64_t>(
                profileAdress + 0x28,
                globals::visuals::worldlevelvalue);
        }
        if (globals::visuals::profile && globals::visuals::xp) {
            utils::Write<uint64_t>(
                profileAdress + 0x4C,
                globals::visuals::xpvalue);
        }
        if (globals::visuals::profile && globals::visuals::playerlevel) {
            utils::Write<uint64_t>(
                profileAdress + 0x24,
                globals::visuals::playerlevel);
        }

    }
}