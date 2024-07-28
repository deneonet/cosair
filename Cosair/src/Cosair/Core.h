#pragma once

#include <memory>

#ifdef _WIN32
#ifdef _WIN64
#define CR_PLATFORM_WINDOWS
#else
#error "x86 Windows is not supported"
#endif
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>

#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported"
#elif TARGET_OS_IPHONE == 1
#define CR_PLATFORM_IOS
#error "IOS is not supported"
#elif TARGET_OS_MAC == 1
#define CR_PLATFORM_MACOS
#error "MacOS is not supported"
#else
#error "Unknown Apple platform"
#endif
#elif defined(__ANDROID__)
#define CR_PLATFORM_ANDROID
#error "Android is not supported"
#elif defined(__linux__)
#define CR_PLATFORM_LINUX
#error "Linux is not supported"
#else
#error "Unknown platform!"
#endif

////////////////////////////
// EXTENSIONS //////////////
////////////////////////////

#ifndef CR_DIST
#define CR_EXT_IMGUI    // Enables imgui
#define CR_EXT_LOGGING  // Enables logging
#endif

#if 0
#define CR_DB_ON_ERRORS  // Calls '__debug_break();' on every error that
// occoured
#endif

#define CR_EXT_BINDLESS_TEXS     // Enables bindless textures
#define CR_EXT_NATIVE_SCRIPTING  // Enables native (C++) scripting, using
                                 // NativeScript components

#ifdef CR_DEBUG
#define CR_EXT_RENDER_STATS  // Access to render statistics, like quad count,
                             // draw calls...
#endif

////////////////////////////
// EXTENSIONS //////////////
////////////////////////////

#if defined CR_DEBUG || defined CR_RELEASE
#define CR_ENABLE_ASSERTS
#endif

#if defined CR_ENABLE_ASSERTS && !defined CR_DISABLE_ASSERTS
#define CR_ASSERT(x, ...)  \
  if (!(x)) {              \
    CR_ERROR(__VA_ARGS__); \
    __debugbreak();        \
  }
#define CR_CORE_ASSERT(x, ...)  \
  if (!(x)) {                   \
    CR_CORE_ERROR(__VA_ARGS__); \
    __debugbreak();             \
  }
#else
#define CR_ASSERT(x, ...)
#define CR_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

namespace cosair {

template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
Scope<T> CreateScope(Args&&... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
Ref<T> CreateRef(Args&&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

}  // namespace cosair