#include <Ursa.h>

#pragma warning(push)
#pragma warning(disable : 6011)
#pragma warning(disable : 26495)
#include "imgui/imgui.h"
#pragma warning(pop)
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class ExampleLayer : public Ursa::Layer {
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f) {
		//Triangle/////////////
		m_TriVertexArray.reset(Ursa::VertexArray::Create());

		float triVertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,		1.0f, 0.4f, 0.7f, 1.0f,
			0.5f, -0.5f, 0.0f,		0.9f, 0.9f, 0.6f, 1.0f,
			0.0f, 0.5f, 0.0f,		0.4f, 0.8f, 0.8f, 1.0f
		};

		Ursa::Ref<Ursa::VertexBuffer> triVertexBuffer;
		triVertexBuffer.reset(Ursa::VertexBuffer::Create(triVertices, sizeof(triVertices)));
		triVertexBuffer->SetLayout({
			{ Ursa::ShaderDataType::Float3, "a_Position" },
			{ Ursa::ShaderDataType::Float4, "a_Color" }
			});
		m_TriVertexArray->AddVertexBuffer(triVertexBuffer);

		uint32_t triIndices[3] = { 0, 1, 2 };
		Ursa::Ref<Ursa::IndexBuffer> triIndexBuffer;
		triIndexBuffer.reset(Ursa::IndexBuffer::Create(triIndices, sizeof(triIndices) / sizeof(uint32_t)));
		m_TriVertexArray->SetIndexBuffer(triIndexBuffer);

		//Square/////////////
		m_SquareVertexArray.reset(Ursa::VertexArray::Create());

		float sqrVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ursa::Ref<Ursa::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Ursa::VertexBuffer::Create(sqrVertices, sizeof(sqrVertices)));
		squareVertexBuffer->SetLayout({
			{ Ursa::ShaderDataType::Float3, "a_Position" },
			{ Ursa::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t sqrIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ursa::Ref<Ursa::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Ursa::IndexBuffer::Create(sqrIndices, sizeof(sqrIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

		m_Shader.reset(Ursa::Shader::Create(vertexSrc, fragmentSrc));

		std::string flatColorVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main() {
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform *  vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			uniform vec3 u_Color;

			void main() {
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader.reset(Ursa::Shader::Create(flatColorVertexSrc, flatColorFragmentSrc));
	
		std::string textureVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main() {
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform *  vec4(a_Position, 1.0);
			}
		)";

		std::string textureFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec2 v_TexCoord;
			
			uniform sampler2D u_Texture;

			void main() {
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader.reset(Ursa::Shader::Create(textureVertexSrc, textureFragmentSrc));

		m_Texture = Ursa::Texture2D::Create("assets/textures/Checkerboard.png");
		m_FireTexture = Ursa::Texture2D::Create("assets/textures/burn.png");

		std::dynamic_pointer_cast<Ursa::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Ursa::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnUpdate(Ursa::TimeStep timeStep) override {
		//URSA_TRACE("Delta time: {0}s, {1}ms", timeStep.GetSeconds(), timeStep.GetMilliSeconds());
		
		if (Ursa::Input::IsKeyPressed(URSA_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * timeStep;
		else if (Ursa::Input::IsKeyPressed(URSA_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * timeStep;
		if (Ursa::Input::IsKeyPressed(URSA_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * timeStep;
		else if (Ursa::Input::IsKeyPressed(URSA_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * timeStep;
		if (Ursa::Input::IsKeyPressed(URSA_KEY_E))
			m_CameraRotation -= m_CamerarRotationSpeed * timeStep;
		if (Ursa::Input::IsKeyPressed(URSA_KEY_Q))
			m_CameraRotation += m_CamerarRotationSpeed * timeStep;

		Ursa::RenderCommand::SetClearColor({ 0.8f, 0.2f, 0.3f, 1.0f });
		Ursa::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Ursa::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Ursa::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Ursa::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Ursa::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, transform);
			}
		}

		m_Texture->Bind();
		Ursa::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_FireTexture->Bind();
		Ursa::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


		// Triangle
		//Ursa::Renderer::Submit(m_Shader, m_TriVertexArray);

		Ursa::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Ursa::Event& event) override {

	}
private:
	Ursa::Ref<Ursa::Shader> m_Shader;
	Ursa::Ref<Ursa::VertexArray> m_TriVertexArray;

	Ursa::Ref<Ursa::Shader> m_FlatColorShader;
	Ursa::Ref<Ursa::Shader> m_TextureShader;
	Ursa::Ref<Ursa::VertexArray> m_SquareVertexArray;
	Ursa::Ref<Ursa::Texture2D> m_Texture;
	Ursa::Ref<Ursa::Texture2D> m_FireTexture;

	Ursa::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraMoveSpeed = 5.0f;
	float m_CamerarRotationSpeed = 60.0f;

	glm::vec3 m_SquareColor = { 0.4f, 0.8f, 0.8f };
};

class Sandbox : public Ursa::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Ursa::Application* Ursa::CreateApplication() {
	return new Sandbox();
}