#include "ursapch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Ursa {
	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		//texid
	};

	struct Renderer2DData {
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;

		Ref<VertexArray> QuadVA;
		Ref<VertexBuffer> QuadVB;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVBBase = nullptr;
		QuadVertex* QuadVBPtr = nullptr;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{	 
		URSA_PROFILE_FUNCTION();

		s_Data.QuadVA = VertexArray::Create();

		s_Data.QuadVB = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);

		s_Data.QuadVBBase = new QuadVertex[s_Data.MaxVertices];
		
		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVA->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_Texture", 0);
	}	 

	void Renderer2D::Shutdown()
	{	 
		URSA_PROFILE_FUNCTION();
		delete[] s_Data.QuadVBBase;
	}	 
		 
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{	 
		URSA_PROFILE_FUNCTION();
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVBPtr = s_Data.QuadVBBase;
	}	 
		 
	void Renderer2D::EndScene()
	{	 
		URSA_PROFILE_FUNCTION();
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVBPtr - (uint8_t*)s_Data.QuadVBBase);
		s_Data.QuadVB->SetData(s_Data.QuadVBBase, dataSize);
		Flush();
	}

	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);
	}
		 
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{	 
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}	 

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec2& tiling, const glm::vec4& tint)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tiling, tint);
	}
		 
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		URSA_PROFILE_FUNCTION();

		s_Data.QuadVBPtr->Position = position;
		s_Data.QuadVBPtr->Color = color;
		s_Data.QuadVBPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVBPtr++;

		s_Data.QuadVBPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVBPtr->Color = color;
		s_Data.QuadVBPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVBPtr++;

		s_Data.QuadVBPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVBPtr->Color = color;
		s_Data.QuadVBPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVBPtr++;

		s_Data.QuadVBPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVBPtr->Color = color;
		s_Data.QuadVBPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVBPtr++;

		s_Data.QuadIndexCount += 6;

		/*
		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.TextureShader->SetFloat2("u_TexScale", {1.0f, 1.0f});
		s_Data.WhiteTexture->Bind();

		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.TextureShader->SetMat4("u_Transform", transform);
		s_Data.VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.VertexArray);
		*/
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture,const glm::vec2& tiling, const glm::vec4& tint)
	{
		URSA_PROFILE_FUNCTION();
		s_Data.TextureShader->SetFloat4("u_Color", tint);
		s_Data.TextureShader->SetFloat2("u_TexScale", tiling);
		texture->Bind();

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.TextureShader->SetMat4("u_Transform", transform);
		s_Data.QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVA);
	}
}