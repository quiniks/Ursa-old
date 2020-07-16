#pragma once

#include "Core.h"
#include "Window.h"
#include "Ursa/LayerStack.h"
#include "Ursa/Events/Event.h"
#include "Ursa/Events/ApplicationEvent.h"


namespace Ursa {

	class URSA_API Application {
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
		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application* s_Instance;
	};

	//Define in client
	Application* CreateApplication();
}