#include "d3d12_command.h"

using namespace ScratchEngine;

D3D12Command::D3D12Command(ID3D12Device8* const device,
                           D3D12_COMMAND_LIST_TYPE type) {
    HRESULT hr{S_OK};

    // Create the command queue.
    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Type = type;
    THROW_IF_FAILED(
        hr = device->CreateCommandQueue(&desc, IID_PPV_ARGS(&command_queue_)));
    if (FAILED(hr)) {
        valid_ = false;
        return;
    }
    NAME_D3D12_OBJECT(command_queue_, type == D3D12_COMMAND_LIST_TYPE_DIRECT
                                          ? L"Direct Command Queue"
                                      : type == D3D12_COMMAND_LIST_TYPE_COMPUTE
                                          ? L"Compute Command Queue"
                                          : L"Command Queue");

    // Create the command allocator per frame.
    for (uint32_t i{0}; i < FRAME_BUFFER_COUNT; i++) {
        CommandFrame& frame{command_frames_[i]};
        THROW_IF_FAILED(hr = device->CreateCommandAllocator(
                            type, IID_PPV_ARGS(&frame.command_allocator)));
        if (FAILED(hr)) {
            valid_ = false;
            return;
        }
        NAME_D3D12_OBJECT_INDEXED(
            frame.command_allocator, i,
            type == D3D12_COMMAND_LIST_TYPE_DIRECT    ? L"Direct Command List"
            : type == D3D12_COMMAND_LIST_TYPE_COMPUTE ? L"Compute Command List"
                                                      : L"Command List");
    }

    // Create the command list.
    THROW_IF_FAILED(hr = device->CreateCommandList(
                        0, type, command_frames_[0].command_allocator, nullptr,
                        IID_PPV_ARGS(&command_list_)));
    if (FAILED(hr)) {
        valid_ = false;
        return;
    }
    THROW_IF_FAILED(
        command_list_->Close());  // It has to be closed before reset.
    NAME_D3D12_OBJECT(command_list_, type == D3D12_COMMAND_LIST_TYPE_DIRECT
                                         ? L"Direct Command List"
                                     : type == D3D12_COMMAND_LIST_TYPE_COMPUTE
                                         ? L"Compute Command List"
                                         : L"Command List");

    // Create the fence.
    THROW_IF_FAILED(hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE,
                                             IID_PPV_ARGS(&fence_)));
    if (FAILED(hr)) {
        valid_ = false;
        return;
    }
    NAME_D3D12_OBJECT(fence_, L"D3D12 Fence");

    // Create the fence event.
    fence_event_ = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
    assert(fence_event_);
}

D3D12Command::~D3D12Command() {
    assert(!command_queue_ && !command_list_ && !fence_);
}

void D3D12Command::BeginFrame() {
    CommandFrame& frame{command_frames_[frame_index_]};
    // wait for the current frame to be signalled.
    frame.Wait(fence_event_, fence_);
    // reset the command allocator/list.
    THROW_IF_FAILED(frame.command_allocator->Reset());
    THROW_IF_FAILED(command_list_->Reset(frame.command_allocator, nullptr));
}

void D3D12Command::EndFrame() {
    THROW_IF_FAILED(command_list_->Close());
    ID3D12CommandList* const command_list[]{command_list_};
    command_queue_->ExecuteCommandLists(_countof(command_list),
                                        &command_list[0]);
    // signal the fence with the new fence value.
    uint64_t& fence_value{fence_value_};
    ++fence_value;
    CommandFrame& frame{command_frames_[frame_index_]};
    frame.fence_value = fence_value;
    command_queue_->Signal(fence_, fence_value);

    frame_index_ = (frame_index_ + 1) % FRAME_BUFFER_COUNT;
}

void D3D12Command::Release() {
    Flush();

    ReleaseResource(fence_);

    fence_value_ = 0;

    CloseHandle(fence_event_);
    fence_event_ = nullptr;

    ReleaseResource(command_queue_);
    ReleaseResource(command_list_);

    for (uint32_t i{0}; i < FRAME_BUFFER_COUNT; i++) {
        command_frames_[i].Release();
    }
}

void D3D12Command::Flush() {
    for (uint32_t i{0}; i < FRAME_BUFFER_COUNT; i++) {
        command_frames_[i].Wait(fence_event_, fence_);
    }
    frame_index_ = 0;
}

void D3D12Command::CommandFrame::Wait(HANDLE fence_event, ID3D12Fence1* fence) {
    assert(fence_event && fence);
    // If the fence completed value is less than the fence value, means the GPU
    // is still executing the command list. Wait for the fence to complete.
    if (fence->GetCompletedValue() < fence_value) {
        // Trigger the fence event when the fence completed value is equal to
        // the fence value.
        THROW_IF_FAILED(fence->SetEventOnCompletion(fence_value, fence_event));
        // Wait for the fence event to be triggered.
        WaitForSingleObject(fence_event, INFINITE);
    }
}

void D3D12Command::CommandFrame::Release() {
    ReleaseResource(command_allocator);
}
