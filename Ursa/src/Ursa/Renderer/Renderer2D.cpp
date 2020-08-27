#include "ursapch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Ursa {

	struct Renderer2DStorage {
		Ref<VertexArray> VertexArray;
		Ref<Shader> Shader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{	 
		s_Data = new Renderer2DStorage;

		s_Data->VertexArray = VertexArray::Create();

		float sqrVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(VertexBuffer::Create(sqrVertices, sizeof(sqrVertices)));
		squareVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		s_Data->VertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t sqrIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(IndexBuffer::Create(sqrIndices, sizeof(sqrIndices) / sizeof(uint32_t)));
		s_Data->VertexArray->SetIndexBuffer(squareIndexBuffer);

		s_Data->Shader = Shader::Create("assets/shaders/FlatColorShader.glsl");
	}	 
		 
	void Renderer2D::Shutdown()
	{	 
		delete s_Data;
	}	 
		 
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{	 
		s_Data->Shader->Bind();
		s_Data->Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		//s_Data->Shader->SetMat4("u_Transform", glm::mat4(1.0f));
	}	 
		 
	void Renderer2D::EndScene()
	{	 
		 
	}	 
		 
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{	 
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}	 
		 
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		s_Data->Shader->Bind();
		s_Data->Shader->SetFloat4("u_Color", color);

		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data->Shader->SetMat4("u_Transform", transform);
		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}
}