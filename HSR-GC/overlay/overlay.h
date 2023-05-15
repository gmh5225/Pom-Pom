#pragma once

#include <ImGui/backends/imgui_impl_dx11.h>
#include <ImGui/backends/imgui_impl_win32.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>

#include "../features/features.h"

namespace overlay {
  void Update();

  void Main();

  void Setup();
} // namespace overlay