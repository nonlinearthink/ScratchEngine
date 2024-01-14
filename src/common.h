#pragma once

#include <cstdint>
#include <cstdio>

#ifndef DISABLE_COPY
#define DISABLE_COPY(class_name)                     \
    explicit class_name(const class_name&) = delete; \
    class_name& operator=(const class_name&) = delete;
#endif

#ifndef DISABLE_MOVE
#define DISABLE_MOVE(class_name)                \
    explicit class_name(class_name&&) = delete; \
    class_name& operator=(class_name&&) = delete;
#endif

#ifndef DISABLE_COPY_AND_MOVE
#define DISABLE_COPY_AND_MOVE(class_name) \
    DISABLE_COPY(class_name) DISABLE_MOVE(class_name)
#endif

#ifdef _DEBUG

#ifndef THROW_IF_FAILED
#define THROW_IF_FAILED(x)                      \
    if (FAILED(x)) {                            \
        char line_number[32];                   \
        sprintf_s(line_number, "%u", __LINE__); \
        OutputDebugStringA("Error in: ");       \
        OutputDebugStringA(__FILE__);           \
        OutputDebugStringA("\nLine: ");         \
        OutputDebugStringA(line_number);        \
        OutputDebugStringA("\n");               \
        OutputDebugStringA(#x);                 \
        OutputDebugStringA("\n");               \
        __debugbreak();                         \
    }
#endif  // !THROW_IF_FAILED

#ifndef NAME_D3D12_OBJECT
#define NAME_D3D12_OBJECT(obj, name)                \
    obj->SetName(name);                             \
    OutputDebugString(L"::D3D12 Object Created: "); \
    OutputDebugString(name);                        \
    OutputDebugString(L"\n");
#endif  // !NAME_D3D12_OBJECT

#ifndef NAME_D3D12_OBJECT_INDEXED
#define NAME_D3D12_OBJECT_INDEXED(obj, n, name)              \
    {                                                        \
        wchar_t full_name[128];                              \
        if (swprintf_s(full_name, L"%s[%u]", name, n) > 0) { \
            obj->SetName(full_name);                         \
            OutputDebugString(L"::D3D12 Object Created: ");  \
            OutputDebugString(full_name);                    \
            OutputDebugString(L"\n");                        \
        }                                                    \
    }
#endif  // !NAME_D3D12_OBJECT_INDEXED

#else

#ifndef THROW_IF_FAILED
#define THROW_IF_FAILED(x) x
#endif  // !THROW_IF_FAILED

#ifndef NAME_D3D12_OBJECT
#define NAME_D3D12_OBJECT(obj, name)
#endif  // !NAME_D3D12_OBJECT

#ifndef NAME_D3D12_OBJECT_INDEXED
#define NAME_D3D12_OBJECT_INDEXED(obj, n, name)
#endif  // !NAME_D3D12_OBJECT_INDEXED

#endif  // !_DEBUG

namespace ScratchEngine {
const uint32_t FRAME_BUFFER_COUNT{3};

// Release a resource having a Release method and set it to nullptr.
template <typename T>
constexpr void ReleaseResource(T*& resource) {
    if (resource) {
        resource->Release();
        resource = nullptr;
    }
}
}  // namespace ScratchEngine
