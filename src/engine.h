#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <intsafe.h>
#include <wrl.h>

#include <cstdio>

#include "helper.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

namespace ScratchEngine {
constexpr D3D_FEATURE_LEVEL MINIMUM_D3D_FEATURN_LEVEL{D3D_FEATURE_LEVEL_11_0};

class Engine {
public:
    // Initialize the engine.
    bool Initialize();

    // Shutdown the engine.
    void Shutdown();

private:
    Microsoft::WRL::ComPtr<IDXGIFactory7> m_factory{nullptr};
    Microsoft::WRL::ComPtr<ID3D12Device8> m_device{nullptr};

    // Get a suitable adapter that supports minimum feature level.
    IDXGIAdapter4 *GetSuitableAdapter();

    // Get the maximum feature level supported by the device.
    D3D_FEATURE_LEVEL GetMaxFeatureLevel(IDXGIAdapter4 *adapter);

    // Initialize fail
    bool InitializeFailed();
};
}  // namespace ScratchEngine