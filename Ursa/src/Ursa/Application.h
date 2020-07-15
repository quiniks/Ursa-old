#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Ursa/Events/ApplicationEvent.h"
#include "Window.h"

namespace Ursa {

	class URSA_API Application {
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	//Define in client
	Application* CreateApplication();
}