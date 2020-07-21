#include <Ursa.h>

class ExampleLayer : public Ursa::Layer {
public:
	ExampleLayer() : Layer("Example") {}
	void OnUpdate() override {
		URSA_INFO("ExampleLayer::Update");
		if (Ursa::Input::IsKeyPressed(URSA_KEY_TAB))
			URSA_TRACE("TAB (poll)");
	}
	void OnEvent(Ursa::Event& event) override {
		if (event.GetEventType() == Ursa::EventType::KeyPressed) {
			Ursa::KeyPressedEvent& e = (Ursa::KeyPressedEvent&)event;
			if (e.GetKeyCode() == URSA_KEY_TAB)
				URSA_TRACE("TAB (event)");
		}
		//Ursa::EventDispatcher dispatcher(event);
		//dispatcher.Dispatch<Ursa::KeyPressedEvent>(std::bind(&ExampleLayer::test, this, std::placeholders::_1));
	}
	//bool test(Ursa::KeyPressedEvent& e) {
	//	URSA_TRACE("HOI");
	//	return true;
	//}
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