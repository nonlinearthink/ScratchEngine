#pragma once

#include <d3d12.h>

#include <cassert>
#include <cstdint>

#include "common.h"

namespace ScratchEngine {
class D3D12Command {
public:
    DISABLE_COPY_AND_MOVE(D3D12Command);
    D3D12Command() = default;
    explicit D3D12Command(ID3D12Device8* const device,
                          D3D12_COMMAND_LIST_TYPE type);
    ~D3D12Command();

    // Check the validity of this object. The users should always call it
    // before using this object. It will return true by default, but false if
    // the constructor of this object has failed.
    constexpr bool valid() const { return valid_; };
    // Get the command queue.
    constexpr ID3D12CommandQueue* const command_queue() const {
        return command_queue_;
    };
    // Get the command list.
    constexpr ID3D12GraphicsCommandList6* const command_list() const {
        return command_list_;
    };
    // Get the current frame index.
    constexpr uint32_t frame_index() const { return frame_index_; };

    // Be called at the beginning of each frame.
    void BeginFrame();
    // Be called at the end of each frame.
    void EndFrame();
    // Release all resources associated with D3D12Command.
    void Release();

private:
    struct CommandFrame {
        ID3D12CommandAllocator* command_allocator{nullptr};
        uint64_t fence_value{0};

        // Wait for the GPU to finish.
        void Wait(HANDLE fence_event, ID3D12Fence1* fence);
        // Release the command allocator.
        void Release();
    };

    bool valid_{true};
    ID3D12CommandQueue* command_queue_{nullptr};
    ID3D12GraphicsCommandList6* command_list_{nullptr};
    ID3D12Fence1* fence_{nullptr};
    uint64_t fence_value_{0};
    HANDLE fence_event_{nullptr};
    CommandFrame command_frames_[FRAME_BUFFER_COUNT]{};
    uint32_t frame_index_{0};

    // Wait all of the command frames to finish.
    void Flush();
};
}  // namespace ScratchEngine
