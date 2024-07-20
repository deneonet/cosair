#pragma once

#ifdef CR_PLATFORM_WINDOWS

extern Cosair::Application* Cosair::CreateApplication();

int main(int argc, char** argv) {
	Cosair::Log::Init();

	auto app = Cosair::CreateApplication();
	app->Run();
	delete app;
}

#endif