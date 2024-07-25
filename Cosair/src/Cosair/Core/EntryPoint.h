#pragma once

#ifdef CR_PLATFORM_WINDOWS

extern Cosair::Application* Cosair::CreateApplication();

int main(int argc, char** argv) {
	Cosair::Log::Init();

	CR_PROFILE_BEGIN_SESSION("Startup", "CosairProfile-Startup.json");
	auto app = Cosair::CreateApplication();
	CR_PROFILE_END_SESSION();

	CR_PROFILE_BEGIN_SESSION("Runtime", "CosairProfile-Runtime.json");
	app->Run();
	CR_PROFILE_END_SESSION();

	CR_PROFILE_BEGIN_SESSION("Shutdown", "CosairProfile-Shutdown.json");
	delete app;
	CR_PROFILE_END_SESSION();
}

#endif