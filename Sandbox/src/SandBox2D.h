#pragma once

#include "Ursa.h"

class SandBox2D : public Ursa::Layer {
public:
	SandBox2D();
	virtual ~SandBox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Ursa::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Ursa::Event& e) override;
private:
	Ursa::OrthographicCameraController m_CameraController;

	//Temporary
	Ursa::Ref<Ursa::VertexArray> m_SquareVertexArray;
	Ursa::Ref<Ursa::Shader> m_FlatColorShader;
	glm::vec3 m_SquareColor = { 0.4f, 0.8f, 0.8f };
};