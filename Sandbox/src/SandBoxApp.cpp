#include <Ursa.h>

class Sandbox : public Ursa::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

Ursa::Application* Ursa::CreateApplication() {
	return new Sandbox();
}