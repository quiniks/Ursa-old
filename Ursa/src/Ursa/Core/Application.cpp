#include "ursapch.h"
#include "Application.h"
#include "Ursa/Renderer/Renderer.h"
#include <GLFW/glfw3.h>

namespace Ursa {
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;
	
	Application::Application(const std::string& name)
	{
		URSA_PROFILE_FUNCTION();
		URSA_CORE_ASSERT(!s_Instance, "Applcation already exists");

		s_Instance = this;

		m_Window = Window::Create(WindowProperties(name));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		URSA_PROFILE_FUNCTION();
		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer) {
		URSA_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer) {
		URSA_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e) {
		URSA_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() {
		URSA_PROFILE_FUNCTION();
		while (m_Running) {
			URSA_PROFILE_SCOPE("Run Loop");
			float time = (float)glfwGetTime(); //"Platform"::GetTime()
			TimeStep timeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized) {
				{
					URSA_PROFILE_SCOPE("LayerStack OnUpdates");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timeStep);
				}

				m_ImGuiLayer->Begin();
				{
					URSA_PROFILE_SCOPE("LayerStack OnImGuiRender");
					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		URSA_PROFILE_FUNCTION();
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}