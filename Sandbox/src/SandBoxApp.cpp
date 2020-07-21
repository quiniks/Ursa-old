#include <Ursa.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

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