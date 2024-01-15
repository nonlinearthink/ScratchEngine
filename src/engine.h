#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "common.h"
#include "d3d12_command.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

namespace ScratchEngine {
constexpr D3D_FEATURE_LEVEL MINIMUM_D3D_FEATURN_LEVEL{D3D_FEATURE_LEVEL_11_0};

class Engine {
public:
    // Initialize the engine.
    bool Initialize();
    // Render a frame.
    void Render();
    // Shutdown the engine.
    void Shutdown();

private:
    IDXGIFactory7* factory_{nullptr};
    ID3D12Device8* main_device_{nullptr};
    D3D12Command d3d12_command_;

    // Get the maximum feature level supported by the device.
    static D3D_FEATURE_LEVEL GetMaxFeatureLevel(IDXGIAdapter4* adapter);

    // Initialize fail
    bool HandleInitializeFailed();
    // Get a suitable adapter that supports minimum feature level.
    IDXGIAdapter4* GetSuitableAdapter();
};
}  // namespace ScratchEngine
