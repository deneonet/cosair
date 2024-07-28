#pragma once

#ifdef CR_PLATFORM_WINDOWS

extern cosair::Application* cosair::CreateApplication();

int main(int argc, char** argv) {
  cosair::Log::Init();

  CR_PROFILE_BEGIN_SESSION("Startup", "CosairProfile-Startup.json");
  auto app = cosair::CreateApplication();
  CR_PROFILE_END_SESSION();

  CR_PROFILE_BEGIN_SESSION("Runtime", "CosairProfile-Runtime.json");
  app->Run();
  CR_PROFILE_END_SESSION();

  CR_PROFILE_BEGIN_SESSION("Shutdown", "CosairProfile-Shutdown.json");
  delete app;
  CR_PROFILE_END_SESSION();
}

#endif