#pragma once

#include <spdlog/spdlog.h>
// clang-format off
#include <spdlog/fmt/ostr.h>

#include "Cosair/Core.h"

namespace cosair {

class Log {
 public:
  static void Init();

  inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
    return core_logger_;
  }
  inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
    return client_logger_;
  }

 private:
  static std::shared_ptr<spdlog::logger> core_logger_;
  static std::shared_ptr<spdlog::logger> client_logger_;
};

}  // namespace cosair

#define CR_CORE_CRITICAL(...)                            \
  ::cosair::Log::GetCoreLogger()->critical(__VA_ARGS__); \
  __debugbreak();

#define CR_CRITICAL(...)                                   \
  ::cosair::Log::GetClientLogger()->critical(__VA_ARGS__); \
  __debugbreak();

#ifdef CR_DB_ON_ERRORS

#define CR_ERROR(...) ::cosair::Log::GetClientLogger()->error(__VA_ARGS__); __debugbreak();
#define CR_CORE_ERROR(...) ::cosair::Log::GetCoreLogger()->error(__VA_ARGS__); __debugbreak();

#else

#define CR_ERROR(...) ::cosair::Log::GetClientLogger()->error(__VA_ARGS__)
#define CR_CORE_ERROR(...) ::cosair::Log::GetCoreLogger()->error(__VA_ARGS__)

#endif

#ifdef CR_EXT_LOGGING

#define CR_CORE_WARN(...) ::cosair::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CR_CORE_INFO(...) ::cosair::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CR_CORE_TRACE(...) ::cosair::Log::GetCoreLogger()->trace(__VA_ARGS__)

#define CR_WARN(...) ::cosair::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CR_INFO(...) ::cosair::Log::GetClientLogger()->info(__VA_ARGS__)
#define CR_TRACE(...) ::cosair::Log::GetClientLogger()->trace(__VA_ARGS__)

#else

#define CR_CORE_WARN(...)
#define CR_CORE_INFO(...)
#define CR_CORE_TRACE(...)

#define CR_WARN(...)
#define CR_INFO(...)
#define CR_TRACE(...)

#endif