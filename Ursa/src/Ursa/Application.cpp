#include "ursapch.h"
#include "Application.h"
#include <glad/glad.h>
#include "Input.h"

namespace Ursa {
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		URSA_CORE_ASSERT(!s_Instance, "Applcation already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		
		//Triangle/////////////
		m_TriVertexArray.reset(VertexArray::Create());

		float triVertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,		1.0f, 0.4f, 0.7f, 1.0f,
			0.5f, -0.5f, 0.0f,		0.9f, 0.9f, 0.6f, 1.0f,
			0.0f, 0.5f, 0.0f,		0.4f, 0.8f, 0.8f, 1.0f
		};

		std::shared_ptr<VertexBuffer> triVertexBuffer;
		triVertexBuffer.reset(VertexBuffer::Create(triVertices, sizeof(triVertices)));
		triVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		});
		m_TriVertexArray->AddVertexBuffer(triVertexBuffer);

		uint32_t triIndices[3] = {0, 1, 2};
		std::shared_ptr<IndexBuffer> triIndexBuffer;
		triIndexBuffer.reset(IndexBuffer::Create(triIndices, sizeof(triIndices) / sizeof(uint32_t)));
		m_TriVertexArray->SetIndexBuffer(triIndexBuffer);
		
		//Square/////////////
		m_SquareVertexArray.reset(VertexArray::Create());

		float sqrVertices[3 * 7] = {
			-0.6f, -0.6f, 0.0f,
			0.6f, -0.6f, 0.0f,
			0.6f, 0.6f, 0.0f,
			-0.6f, 0.6f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(VertexBuffer::Create(sqrVertices, sizeof(sqrVertices)));
		squareVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
		});
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t sqrIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(IndexBuffer::Create(sqrIndices, sizeof(sqrIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;

			void main() {
				//color = vec4(0.019, 0.658, 1.0, 1.0);
				//color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		std::string vertexSrc2 = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main() {
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			void main() {
				color = vec4(0.9, 0.9, 0.6, 1.0);
			}
		)";

		m_Shader2.reset(new Shader(vertexSrc2, fragmentSrc2));
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}


	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() {
		while (m_Running) {
			glClearColor(0.8f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader2->Bind();
			m_SquareVertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_SquareVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			m_Shader->Bind();
			m_TriVertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_TriVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}