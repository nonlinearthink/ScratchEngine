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

#else

#ifndef THROW_IF_FAILED
#define THROW_IF_FAILED(x) x
#endif  // !THROW_IF_FAILED

#endif  // !_DEBUG

#ifdef _DEBUG

#ifndef NAME_D3D12_OBJECT
#define NAME_D3D12_OBJECT(obj, name)                \
    obj->SetName(name);                             \
    OutputDebugStringA("::D3D12 Object Created: "); \
    OutputDebugStringA(name);                       \
    OutputDebugStringA("\n");
#endif  // !NAME_D3D12_OBJECT

#else

#ifndef NAME_D3D12_OBJECT
#define NAME_D3D12_OBJECT(obj, name) obj->SetName(name);
#endif  // !NAME_D3D12_OBJECT

#endif

// Release a resource having a Release method and set it to nullptr.
template <typename T>
constexpr void ReleaseResource(T *&resource) {
    if (resource) {
        resource->Release();
        resource = nullptr;
    }
}
