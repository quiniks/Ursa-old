#pragma once

#ifdef URSA_PLATFORM_WINDOWS

extern Ursa::Application* Ursa::CreateApplication();

int main(int argc, char** argv) {
	Ursa::Log::Init();
	URSA_CORE_WARN("Initialised Log");
	URSA_INFO("Boop!");

	auto app = Ursa::CreateApplication();
	app->Run();
	delete app;
}

#endif // URSA_PLATFORM_WINDOWS
