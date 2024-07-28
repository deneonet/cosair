#include "crpch.h"

#include "log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace cosair {

std::shared_ptr<spdlog::logger> Log::core_logger_;
std::shared_ptr<spdlog::logger> Log::client_logger_;

void Log::Init() {
  spdlog::set_pattern("%^[%T] %n: %v%$");

  core_logger_ = spdlog::stdout_color_mt("Cosair");
  core_logger_->set_level(spdlog::level::trace);

  client_logger_ = spdlog::stdout_color_mt("App");
  client_logger_->set_level(spdlog::level::trace);
}

}  // namespace cosair