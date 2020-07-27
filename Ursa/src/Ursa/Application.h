#pragma once

#include "Core.h"
#include "Window.h"

#include "Ursa/LayerStack.h"
#include "Ursa/Events/Event.h"
#include "Ursa/Events/ApplicationEvent.h"
#include "Ursa/ImGui/ImGuiLayer.h"
#include "Ursa/Renderer/Shader.h"
#include "Ursa/Renderer/Buffer.h"

namespace Ursa {

	class Application {
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr <VertexBuffer> m_VertexBuffer;
		std::unique_ptr <IndexBuffer> m_IndexBuffer;
	private:
		static Application* s_Instance;
	};

	//Define in client
	Application* CreateApplication();
}