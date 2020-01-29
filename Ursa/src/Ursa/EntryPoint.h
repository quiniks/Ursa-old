#pragma once

#ifdef URSA_PLATFORM_WINDOWS

extern Ursa::Application* Ursa::CreateApplication();

int main(int argc, char** argv) {
	printf("Starting the Ursa Engine!");
	auto app = Ursa::CreateApplication();
	app->Run();
	delete app;
}

#endif // URSA_PLATFORM_WINDOWS
