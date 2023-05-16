#pragma once
#include <windows.h>
#include "overlay.h"
#include <string>

// namespace overlay {
void overlay::Update() {
  if (!globals::show_menu)
    return;

  static ImGuiWindowFlags classFinderWindowFlags =
      ImGuiWindowFlags_AlwaysAutoResize;

  ImGui::Begin("Pom-Pom", 0, classFinderWindowFlags);

  ImGui::BeginTabBar("##tabs");

  if (ImGui::BeginTabItem("World")) {
    ImGui::Checkbox("Speed Modifier", &globals::world::speed_hack);

    if (globals::world::speed_hack) {
      ImGui::SliderFloat("Global", &globals::world::global_speed, 0.1f, 10.f,
                         "%.1f");
      ImGui::SliderFloat("Dialogue", &globals::world::dialogue_speed, 0.1f,
                         10.f, "%.1f");
    }

    ImGui::Checkbox("Peeking", &globals::world::peeking);

    ImGui::Checkbox("Auto-Dialogue", &globals::world::auto_dialogue);

    if (globals::world::auto_dialogue) {
      ImGui::Text("also works on hotkey (CAPSLOCK)");
      ImGui::Checkbox("Mouse Mode", &globals::world::mouse_mode);
    }

    ImGui::Checkbox("Invisibility", &globals::world::invisibility);

    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem("Battle")) {
    ImGui::Checkbox("Speed Modifier", &globals::battle::speed_hack);

    if (globals::battle::speed_hack) {
      ImGui::SliderFloat("Battle", &globals::battle::battle_speed, 0.1f, 10.f,
                         "%.1f");
    }

    // ImGui::Checkbox("Auto-Battle Unlock",
    // &globals::battle::auto_battle_unlock);

    ImGui::Checkbox("Force Auto-Battle", &globals::battle::force_battle);

    if (globals::battle::force_battle) {
      ImGui::Text("if you enabled it in battle then you need to do some "
                  "action to make "
                  "it work");
    }

    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem("Other")) {
    ImGui::Checkbox("FPS Unlock", &globals::other::fps_unlock);

    if (globals::other::fps_unlock) {
      ImGui::InputInt("FPS", &globals::other::fps);
    }

    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem("Debug")) {
    ImGui::Text("Phase: %s", hooks::game::get_phase_in_text());
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem("Settings")) {
    static bool pinWindow = false;

    if (ImGui::Checkbox("Pin Window", &pinWindow)) {
      if (pinWindow) {
        classFinderWindowFlags |= ImGuiWindowFlags_NoMove;
        classFinderWindowFlags |= ImGuiWindowFlags_NoResize;
      } else {
        classFinderWindowFlags &= ~ImGuiWindowFlags_NoMove;
        classFinderWindowFlags &= ~ImGuiWindowFlags_NoResize;
      }
    }

    ImGui::SameLine();

    if (ImGui::Button("Reset Position"))
      ImGui::SetWindowPos(ImVec2(0, 0));

    ImGui::SameLine();

    if (ImGui::Button("Unload"))
      globals::unload = true;

    if (ImGui::Button("Save Config")) {
      config::Save();
    }

    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem("About")) {
    ImGui::Text("About this project");
    ImGui::Text("Thanks to Z4ee for the project source code");
    ImGui::Text("This project is free!");
    ImGui::Text("List of Contributing Developers");
    ImGui::Text("b-e-y");
    ImGui::Text("ELJoOker2004");
    ImGui::Text("AyamiAlince");
    ImGui::Text("ky-ler");
    ImGui::Text("Join the Discord server for Support/Updates");
    ImGui::TextColored(ImVec4(0.0f, 0.5f, 1.0f, 1.0f), "Discord Invite: ");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                       "https://discord.gg/vC9a8ba4b8");
    if (ImGui::IsItemClicked()) {
      // Open the website
      std::string url = "https://discord.gg/vC9a8ba4b8";
      std::wstring wideUrl(url.begin(), url.end());
      ShellExecute(NULL, NULL, wideUrl.c_str(), NULL, NULL, SW_SHOWNORMAL);
    }


    ImGui::EndTabItem();
  }

  ImGui::EndTabBar();
  ImGui::End();
}

void overlay::Main() {
  uint64_t game_assembly = 0, unity_player = 0;

  while (!game_assembly && !unity_player) {
    game_assembly =
        reinterpret_cast<uint64_t>(GetModuleHandleA("gameassembly.dll"));
    unity_player =
        reinterpret_cast<uint64_t>(GetModuleHandleA("unityplayer.dll"));
  }

  CreateThread(0, 0, (LPTHREAD_START_ROUTINE)features::dialogue::AutoDialogue,
               0, 0, 0);

  while (true) {
    features::speedhack::UpdateWorld(game_assembly, unity_player);
    features::speedhack::UpdateBattle(game_assembly, unity_player);
    features::other::Update(unity_player);

    HWND target_window = FindWindowA("UnityWndClass", nullptr);

    if (GetForegroundWindow() == target_window &&
        GetAsyncKeyState(globals::hotkeys::world_speed) & 1) {
      globals::world::speed_hack = !globals::world::speed_hack;
    } else if (GetForegroundWindow() == target_window &&
               GetAsyncKeyState(globals::hotkeys::battle_speed) & 1) {
      globals::battle::speed_hack = !globals::battle::speed_hack;
    } else if (GetForegroundWindow() == target_window &&
               GetAsyncKeyState(globals::hotkeys::auto_dialogue) & 1) {
      globals::world::auto_dialogue = !globals::world::auto_dialogue;
    } else if (GetForegroundWindow() == target_window &&
               GetAsyncKeyState(globals::hotkeys::invisibility) & 1) {
      globals::world::invisibility = !globals::world::invisibility;
    } else if (GetForegroundWindow() == target_window &&
               GetAsyncKeyState(globals::hotkeys::force_auto_battle) & 1) {
      globals::battle::force_battle = !globals::battle::force_battle;
    } else if (GetForegroundWindow() == target_window &&
               GetAsyncKeyState(globals::hotkeys::peeking) & 1) {
      globals::world::peeking = !globals::world::peeking;
    }

    Sleep(500);
  }
}

void overlay::Setup() {
  hooks::Setup();

  auto hook = ImGuiContextHook();
  hook.Callback = (ImGuiContextHookCallback)Update;
  hook.Type = ImGuiContextHookType_NewFramePost;
  ImGui::AddContextHook(ImGui::GetCurrentContext(), &hook);
}
//} // namespace overlay