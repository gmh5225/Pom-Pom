#include "features.h"

namespace features::dialogue {
  void AutoDialogue() {
    HWND target_window = 0;

    while (!target_window)
      target_window = FindWindowA("UnityWndClass", nullptr);

    while (true) {
      if (hooks::game::phase != 12) {
        Sleep(500);
        continue;
      }

      if (globals::world::auto_dialogue && !globals::show_menu) {
        if (hooks::game::get_is_in_dialog() || GetAsyncKeyState(VK_CAPITAL)) {
          Sleep(16);

          if (GetForegroundWindow() == target_window) {
            if (!globals::world::mouse_mode) {
              keybd_event(VK_SPACE, 0, 0, 0);
              Sleep(20);
              keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
              Sleep(20);

            } else {
              POINT cursor_pos;
              GetCursorPos(&cursor_pos);
              mouse_event(MOUSEEVENTF_LEFTDOWN, cursor_pos.x, cursor_pos.y, 0,
                          0);
              Sleep(10);
              mouse_event(MOUSEEVENTF_LEFTUP, cursor_pos.x, cursor_pos.y, 0, 0);
              Sleep(20);

            }
          }
        }
      } else {
        Sleep(100);
      }
    }
  }
} // namespace features::dialogue