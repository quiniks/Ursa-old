#include "SandBox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SandBox2D::SandBox2D()
	:Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f, true)
{

}

void SandBox2D::OnAttach()
{
	m_Texture = Ursa::Texture2D::Create("assets/textures/Checkerboard.png");
}

void SandBox2D::OnDetach()
{

}

void SandBox2D::OnUpdate(Ursa::TimeStep ts)
{
	//Update
	m_CameraController.OnUpdate(ts);
	//Render
	Ursa::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Ursa::RenderCommand::Clear();

	Ursa::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Ursa::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.7f, 0.7f }, 45.0f, { 0.8f, 0.2f, 0.3f, 1.0f});
	Ursa::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.7f }, 0.0f, { 0.4f, 0.8f, 0.8f, 1.0f });
	Ursa::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, 0.0f, m_Texture);
	Ursa::Renderer2D::EndScene();
}

void SandBox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void SandBox2D::OnEvent(Ursa::Event& e)
{
	m_CameraController.OnEvent(e);
}
