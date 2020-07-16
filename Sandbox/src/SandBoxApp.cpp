#include <Ursa.h>

class ExampleLayer : public Ursa::Layer {
public:
	ExampleLayer() : Layer("Example") {}
	void OnUpdate() override {
		URSA_INFO("ExampleLayer::Update");
	}
	void OnEvent(Ursa::Event& event) override {
		URSA_TRACE("{0}", event);
	}
};

class Sandbox : public Ursa::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Ursa::ImGuiLayer());
	}

	~Sandbox() {

	}
};

Ursa::Application* Ursa::CreateApplication() {
	return new Sandbox();
}