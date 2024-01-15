#include "engine.h"

using namespace ScratchEngine;
using namespace Microsoft::WRL;

bool Engine::Initialize() {
    if (main_device_) Shutdown();

    UINT dxgi_factory_flags{0};

#ifdef _DEBUG
    // Enable the D3D12 debug layer.
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
            debugController->EnableDebugLayer();
        }

        // Enable DXGI Factory debug.
        dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
    }
#endif

    HRESULT hr{S_OK};

    THROW_IF_FAILED(
        hr = CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&factory_)));
    if (FAILED(hr)) return HandleInitializeFailed();

    // Get a suitable adapter.
    ComPtr<IDXGIAdapter4> main_adapter;
    main_adapter.Attach(GetSuitableAdapter());
    if (!main_adapter) return HandleInitializeFailed();

    // Check the maximum feature level.
    D3D_FEATURE_LEVEL maximum_feature_level{
        Engine::GetMaxFeatureLevel(main_adapter.Get())};
    if (maximum_feature_level <= MINIMUM_D3D_FEATURN_LEVEL) {
        return HandleInitializeFailed();
    }

    // Create the main device.
    THROW_IF_FAILED(hr = D3D12CreateDevice(main_adapter.Get(),
                                           maximum_feature_level,
                                           IID_PPV_ARGS(&main_device_)));
    if (FAILED(hr)) return HandleInitializeFailed();

    NAME_D3D12_OBJECT(main_device_, L"Main D3D12 Device");

    // Placement new, D3D12Command should't have some member pointing to heap.
    new (&d3d12_command_)
        D3D12Command(*main_device_, D3D12_COMMAND_LIST_TYPE_DIRECT);
    if (!d3d12_command_.valid()) return HandleInitializeFailed();

#ifdef _DEBUG
    {
        ComPtr<ID3D12InfoQueue> info_queue;
        THROW_IF_FAILED(
            hr = main_device_->QueryInterface(IID_PPV_ARGS(&info_queue)));
        // Enable severities.
        info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
    }
#endif

    return true;
}

void Engine::Render() {
    // Wait for the GPU to finish. Reset the allocator once GPU is done with it.
    // This will frees the memory that was used to store commands.
    d3d12_command_.BeginFrame();

    ID3D12GraphicsCommandList6 &command_list{d3d12_command_.command_list()};

    // Done recording commands and execute commands now. Signal and increase the
    // fence value for next frame.
    d3d12_command_.EndFrame();
}

void Engine::Shutdown() {
    d3d12_command_.Release();

    ReleaseResource(factory_);

#ifdef _DEBUG
    {
        if (!main_device_) return;

        {
            ComPtr<ID3D12InfoQueue> info_queue;
            THROW_IF_FAILED(
                main_device_->QueryInterface(IID_PPV_ARGS(&info_queue)));
            // Disable severities.
            info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION,
                                           false);
            info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, false);
            info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING,
                                           false);
        }

        ComPtr<ID3D12DebugDevice2> debugDevice;
        THROW_IF_FAILED(
            main_device_->QueryInterface(IID_PPV_ARGS(&debugDevice)));
        ReleaseResource(main_device_);
        // Report live objects that causes memory leaks when exit.
        THROW_IF_FAILED(debugDevice->ReportLiveDeviceObjects(
            D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL |
            D3D12_RLDO_IGNORE_INTERNAL));
    }
#else
    ReleaseResource(main_device_);
#endif
}

D3D_FEATURE_LEVEL Engine::GetMaxFeatureLevel(IDXGIAdapter4 *adapter) {
    constexpr D3D_FEATURE_LEVEL feature_levels[4]{
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_12_1,
    };

    D3D12_FEATURE_DATA_FEATURE_LEVELS feature_levels_info;
    feature_levels_info.NumFeatureLevels = _countof(feature_levels);
    feature_levels_info.pFeatureLevelsRequested = feature_levels;

    ComPtr<ID3D12Device> device;
    THROW_IF_FAILED(D3D12CreateDevice(adapter, MINIMUM_D3D_FEATURN_LEVEL,
                                      IID_PPV_ARGS(&device)));
    THROW_IF_FAILED(device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS,
                                                &feature_levels_info,
                                                sizeof(feature_levels_info)));
    return feature_levels_info.MaxSupportedFeatureLevel;
}

bool Engine::HandleInitializeFailed() {
    Shutdown();
    return false;
}

IDXGIAdapter4 *Engine::GetSuitableAdapter() {
    IDXGIAdapter4 *adapter{nullptr};

    // Get adapters in descending order of preference.
    for (UINT i{0}; factory_->EnumAdapterByGpuPreference(
                        i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
                        IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND;
         ++i) {
        // Pick the first adapter that supports the minimum feature level.
        if (SUCCEEDED(D3D12CreateDevice(adapter, MINIMUM_D3D_FEATURN_LEVEL,
                                        _uuidof(ID3D12Device), nullptr))) {
            return adapter;
        }
        ReleaseResource(adapter);
    }

    return nullptr;
}
