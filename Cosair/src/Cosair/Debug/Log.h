#pragma once

#include "Cosair/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Cosair {

	class Log {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

#ifdef CR_DIST

#define CR_CORE_CRITICAL(...)
#define CR_CORE_DERROR(...)
#define CR_CORE_ERROR(...)
#define CR_CORE_WARN(...)
#define CR_CORE_INFO(...)
#define CR_CORE_TRACE(...)

#define CR_CRITICAL(...)
#define CR_ERROR(...)
#define CR_WARN(...)
#define CR_INFO(...)
#define CR_TRACE(...)

#else

#define CR_CORE_CRITICAL(...)  ::Cosair::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define CR_CORE_DERROR(...)    ::Cosair::Log::GetCoreLogger()->error(__VA_ARGS__); __debugbreak();
#define CR_CORE_ERROR(...)     ::Cosair::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CR_CORE_WARN(...)      ::Cosair::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CR_CORE_INFO(...)      ::Cosair::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CR_CORE_TRACE(...)     ::Cosair::Log::GetCoreLogger()->trace(__VA_ARGS__)

#define CR_CRITICAL(...)  ::Cosair::Log::GetClientLogger()->critical(__VA_ARGS__)
#define CR_ERROR(...)     ::Cosair::Log::GetClientLogger()->error(__VA_ARGS__)
#define CR_WARN(...)      ::Cosair::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CR_INFO(...)      ::Cosair::Log::GetClientLogger()->info(__VA_ARGS__)
#define CR_TRACE(...)     ::Cosair::Log::GetClientLogger()->trace(__VA_ARGS__)

#endif