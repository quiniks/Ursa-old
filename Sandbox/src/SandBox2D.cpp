#include "SandBox2D.h"
#pragma warning(push)
#pragma warning(disable : 6011)
#pragma warning(disable : 26495)
#include "imgui/imgui.h"
#pragma warning(pop)

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SandBox2D::SandBox2D()
	:Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f, true)
{

}

void SandBox2D::OnAttach()
{
	URSA_PROFILE_FUNCTION();
	m_Texture = Ursa::Texture2D::Create("assets/textures/Checkerboard.png");
}

void SandBox2D::OnDetach()
{
	URSA_PROFILE_FUNCTION();
}

void SandBox2D::OnUpdate(Ursa::TimeStep ts)
{
	URSA_PROFILE_FUNCTION();
	//Update
	m_CameraController.OnUpdate(ts);
	//Render
	Ursa::Renderer2D::ResetStats();
	{
		URSA_PROFILE_SCOPE("Renderer Prep");
		Ursa::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Ursa::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 20.0f;
		URSA_PROFILE_SCOPE("Renderer Draw");
		Ursa::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Ursa::Renderer2D::DrawQuad({ -0.5f, 0.0f }, { 0.7f, 0.7f }, glm::radians(rotation), { 0.8f, 0.2f, 0.3f, 1.0f });
		Ursa::Renderer2D::DrawQuad({ 0.5f, 0.0f }, { 0.5f, 0.7f }, 0.0f, { 0.4f, 0.8f, 0.8f, 1.0f });
		Ursa::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, 0.0f, m_Texture, {10.0f, 10.0f}, { 0.1f, 0.1f, 0.1f, 1.0f });
		//Ursa::Renderer2D::EndScene();

		//Ursa::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.0f, (y + 5.0f) / 10.0f, 0.5f };
				Ursa::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, 0.0f, color);
			}
		}
		Ursa::Renderer2D::EndScene();
	}
}

void SandBox2D::OnImGuiRender()
{
	URSA_PROFILE_FUNCTION();
	ImGui::Begin("SandBox");
	auto stats = Ursa::Renderer2D::GetStats();
	ImGui::Text("Renderer2D stats");
	ImGui::Text("Draw calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	//ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void SandBox2D::OnEvent(Ursa::Event& e)
{
	m_CameraController.OnEvent(e);
}
