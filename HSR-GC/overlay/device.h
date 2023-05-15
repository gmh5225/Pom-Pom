#pragma once

#include <Direct3D/Direct3D.h>

#include <MinHook/MinHook.h>

#include "overlay.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window,
                                                             UINT message,
                                                             WPARAM wideParam,
                                                             LPARAM longParam);

namespace device {
  inline ID3D11Device *pDevice = NULL;
  inline ID3D11DeviceContext *pContext = NULL;
  inline ID3D11RenderTargetView *mainRenderTargetView = NULL;
  inline WNDPROC oWndProc = NULL;
  inline HWND hWnd = NULL;

  using PresentFn = HRESULT(__stdcall *)(IDXGISwapChain *pSwapChain,
                                         UINT SyncInterval, UINT Flags);
  using ResizeBuffersFn = HRESULT(__stdcall *)(IDXGISwapChain *pSwapChain,
                                               UINT BufferCount, UINT Width,
                                               UINT Height,
                                               DXGI_FORMAT NewFormat,
                                               UINT SwapChainFlags);
  inline PresentFn oPresent = nullptr;
  inline ResizeBuffersFn oResizeBuffers = nullptr;

  inline LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam,
                                LPARAM lParam) {
    if (globals::unload) {
      return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
    }

    if (uMsg == WM_KEYUP && wParam == VK_F1) {
      globals::show_menu = !globals::show_menu;
    }

    ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

    if (!globals::show_menu) {
      return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
    }

    return true;
  }

  inline HRESULT Present(IDXGISwapChain *pSwapChain, UINT SyncInterval,
                         UINT Flags) {
    if (globals::unload) {
      return oPresent(pSwapChain, SyncInterval, Flags);
    }

    static auto bMakeOnce = [&]() -> bool {
      if (FAILED(pSwapChain->GetDevice(__uuidof(ID3D11Device),
                                       (void **)&pDevice))) {
        return false;
      }

      pDevice->GetImmediateContext(&pContext);
      DXGI_SWAP_CHAIN_DESC sd;
      pSwapChain->GetDesc(&sd);
      hWnd = sd.OutputWindow;
      ID3D11Texture2D *pBackBuffer = NULL;
      pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                            (LPVOID *)&pBackBuffer);
      D3D11_RENDER_TARGET_VIEW_DESC desc = {};
      desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
      pDevice->CreateRenderTargetView(pBackBuffer, &desc,
                                      &mainRenderTargetView);
      pBackBuffer->Release();

      oWndProc =
          (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

      ImGui::CreateContext();
      ImGui::GetIO().IniFilename = nullptr;
      ImGui::GetIO().LogFilename = nullptr;

      {
        ImGuiStyle &style = ImGui::GetStyle();
        style.Alpha = 1.0f;
        style.DisabledAlpha = 0.6000000238418579f;
        style.WindowPadding = ImVec2(8.0f, 8.0f);
        style.WindowRounding = 4.0f;
        style.WindowBorderSize = 0.0f;
        style.WindowMinSize = ImVec2(32.0f, 32.0f);
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style.WindowMenuButtonPosition = ImGuiDir_Left;
        style.ChildRounding = 0.0f;
        style.ChildBorderSize = 1.0f;
        style.PopupRounding = 4.0f;
        style.PopupBorderSize = 1.0f;
        style.FramePadding = ImVec2(8.0f, 6.0f);
        style.FrameRounding = 5.5f;
        style.FrameBorderSize = 0.0f;
        style.ItemSpacing = ImVec2(8.0f, 4.0f);
        style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
        style.CellPadding = ImVec2(4.0f, 2.0f);
        style.IndentSpacing = 21.0f;
        style.ColumnsMinSpacing = 6.0f;
        style.ScrollbarSize = 11.0f;
        style.ScrollbarRounding = 2.5f;
        style.GrabMinSize = 10.0f;
        style.GrabRounding = 2.0f;
        style.TabRounding = 3.5f;
        style.TabBorderSize = 0.0f;
        style.TabMinWidthForCloseButton = 0.0f;
        style.ColorButtonPosition = ImGuiDir_Right;
        style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
        style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_TextDisabled] =
            ImVec4(0.5921568870544434f, 0.5921568870544434f,
                   0.5921568870544434f, 1.0f);
        style.Colors[ImGuiCol_WindowBg] =
            ImVec4(0.1450980454683304f, 0.1450980454683304f,
                   0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_ChildBg] =
            ImVec4(0.1450980454683304f, 0.1450980454683304f,
                   0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_PopupBg] =
            ImVec4(0.1450980454683304f, 0.1450980454683304f,
                   0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_Border] =
            ImVec4(0.3058823645114899f, 0.3058823645114899f,
                   0.3058823645114899f, 1.0f);
        style.Colors[ImGuiCol_BorderShadow] =
            ImVec4(0.3058823645114899f, 0.3058823645114899f,
                   0.3058823645114899f, 1.0f);
        style.Colors[ImGuiCol_FrameBg] =
            ImVec4(0.2000000029802322f, 0.2000000029802322f,
                   0.2156862765550613f, 1.0f);
        style.Colors[ImGuiCol_FrameBgHovered] =
            ImVec4(0.2558823645114899f, 0.2558823645114899f,
                   0.2558823645114899f, 1.0f);
        style.Colors[ImGuiCol_FrameBgActive] =
            ImVec4(0.3000000029802322f, 0.3000000029802322f,
                   0.3156862765550613f, 1.0f);
        style.Colors[ImGuiCol_TitleBg] =
            ImVec4(0.1450980454683304f, 0.1450980454683304f,
                   0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_TitleBgActive] =
            ImVec4(0.1450980454683304f, 0.1450980454683304f,
                   0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_TitleBgCollapsed] =
            ImVec4(0.1450980454683304f, 0.1450980454683304f,
                   0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_MenuBarBg] =
            ImVec4(0.2000000029802322f, 0.2000000029802322f,
                   0.2156862765550613f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarBg] =
            ImVec4(0.2000000029802322f, 0.2000000029802322f,
                   0.2156862765550613f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(
            0.321568638086319f, 0.321568638086319f, 0.3333333432674408f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] =
            ImVec4(0.3529411852359772f, 0.3529411852359772f,
                   0.3725490272045135f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] =
            ImVec4(0.3529411852359772f, 0.3529411852359772f,
                   0.3725490272045135f, 1.0f);
        style.Colors[ImGuiCol_CheckMark] =
            ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
        style.Colors[ImGuiCol_SliderGrab] =
            ImVec4(0.1137254908680916f, 0.5921568870544434f,
                   0.9254902005195618f, 1.0f);
        //style.Colors[ImGuiCol_SliderGrabActive] =
        //    ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
        style.Colors[ImGuiCol_Button] =
            ImVec4(0.2000000029802322f, 0.2000000029802322f,
                   0.2156862765550613f, 1.0f);
        style.Colors[ImGuiCol_ButtonHovered] =
            ImVec4(0.1137254908680916f, 0.5921568870544434f,
                   0.9254902005195618f, 1.0f);
        style.Colors[ImGuiCol_ButtonActive] =
            ImVec4(0.1137254908680916f, 0.5921568870544434f,
                   0.9254902005195618f, 1.0f);
        style.Colors[ImGuiCol_Header] =
            ImVec4(0.2000000029802322f, 0.2000000029802322f,
                   0.2156862765550613f, 1.0f);
        style.Colors[ImGuiCol_HeaderHovered] =
            ImVec4(0.1137254908680916f, 0.5921568870544434f,
                   0.9254902005195618f, 1.0f);
        style.Colors[ImGuiCol_HeaderActive] =
            ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
        style.Colors[ImGuiCol_Separator] =
            ImVec4(0.3058823645114899f, 0.3058823645114899f,
                   0.3058823645114899f, 1.0f);
        style.Colors[ImGuiCol_SeparatorHovered] =
            ImVec4(0.3058823645114899f, 0.3058823645114899f,
                   0.3058823645114899f, 1.0f);
        style.Colors[ImGuiCol_SeparatorActive] =
            ImVec4(0.3058823645114899f, 0.3058823645114899f,
                   0.3058823645114899f, 1.0f);
        style.Colors[ImGuiCol_ResizeGrip] =
            ImVec4(0.1450980454683304f, 0.1450980454683304f,
                   0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_ResizeGripHovered] =
            ImVec4(0.2000000029802322f, 0.2000000029802322f,
                   0.2156862765550613f, 1.0f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(
            0.321568638086319f, 0.321568638086319f, 0.3333333432674408f, 1.0f);
        style.Colors[ImGuiCol_Tab] =
            ImVec4(0.1450980454683304f, 0.1450980454683304f,
                   0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_TabHovered] =
            ImVec4(0.1137254908680916f, 0.5921568870544434f,
                   0.9254902005195618f, 1.0f);
        style.Colors[ImGuiCol_TabActive] =
            ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
        style.Colors[ImGuiCol_TabUnfocused] =
            ImVec4(0.1450980454683304f, 0.1450980454683304f,
                   0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_TabUnfocusedActive] =
            ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
        style.Colors[ImGuiCol_TextSelectedBg] =
            ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
        style.Colors[ImGuiCol_DragDropTarget] =
            ImVec4(0.1450980454683304f, 0.1450980454683304f,
                   0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_NavHighlight] =
            ImVec4(0.1450980454683304f, 0.1450980454683304f,
                   0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_NavWindowingHighlight] =
            ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
        style.Colors[ImGuiCol_NavWindowingDimBg] =
            ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f,
                   0.2000000029802322f);
        style.Colors[ImGuiCol_ModalWindowDimBg] =
            ImVec4(0.1450980454683304f, 0.1450980454683304f,
                   0.1490196138620377f, 1.0f);
      }

      ImGui_ImplWin32_Init(hWnd);
      ImGui_ImplDX11_Init(pDevice, pContext);

      ImGui::GetIO().Fonts->AddFontFromFileTTF(
          "c:\\windows\\fonts\\tahoma.ttf", 16, NULL,
          ImGui::GetIO().Fonts->GetGlyphRangesChineseFull());

      overlay::Setup();

      return true;
    }();
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::Render();
    pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    return oPresent(pSwapChain, SyncInterval, Flags);
  }

  inline HRESULT ResizeBuffers(IDXGISwapChain *pSwapChain, UINT BufferCount,
                               UINT Width, UINT Height, DXGI_FORMAT NewFormat,
                               UINT SwapChainFlags) {
    if (globals::unload) {
      return oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat,
                            SwapChainFlags);
    }

    if (mainRenderTargetView) {
      pContext->OMSetRenderTargets(0, 0, 0);
      mainRenderTargetView->Release();
      mainRenderTargetView = NULL;
    }

    HRESULT hr = oResizeBuffers(pSwapChain, BufferCount, Width, Height,
                                NewFormat, SwapChainFlags);
    ID3D11Texture2D *pBuffer = NULL;
    pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&pBuffer);
    pDevice->CreateRenderTargetView(pBuffer, nullptr, &mainRenderTargetView);
    pBuffer->Release();
    pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)Width;
    vp.Height = (FLOAT)Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pContext->RSSetViewports(1, &vp);
    return hr;
  }

  inline bool Setup() {
    static auto CreateHook = [](LPVOID pTarget, LPVOID pDetour,
                                LPVOID *ppOriginal) -> bool {
      return MH_CreateHook(pTarget, pDetour, ppOriginal) == MH_OK &&
             MH_EnableHook(pTarget) == MH_OK;
    };

    if (!CreateHook(Direct3D.Present, device::Present,
                    reinterpret_cast<void **>(&device::oPresent))) {
      return false;
    }

    if (!CreateHook(Direct3D.ResizeBuffers, device::ResizeBuffers,
                    reinterpret_cast<void **>(&device::oResizeBuffers))) {
      return false;
    }

    return true;
  }
} // namespace device