#include "EditorLayer.h"
#pragma warning(push)
#pragma warning(disable : 6011)
#pragma warning(disable : 26495)
#include "imgui/imgui.h"
#pragma warning(pop)

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Ursa {

	static const uint32_t s_MapWidth = 9;
	static const char* s_MapTiles =
		"GGGGWGGGG"
		"GTGGWGGGG"
		"GGGGWGGTG"
		"GGGGWGGGG"
		"GGGGBGGGG"
		"GGGGWGGGG"
		"GGTGWGGGG"
		"GGGGWGGAG"
		"GGGGWGGGG";

	EditorLayer::EditorLayer()
		:Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, true)
	{

	}

	void EditorLayer::OnAttach()
	{
		URSA_PROFILE_FUNCTION();
		m_CheckerTexture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_TileSheet = Texture2D::Create("assets/textures/colored_transparent_packed.png");
		m_UrsaTitle = Texture2D::Create("assets/textures/Ursa.png");

		m_MapWidth = s_MapWidth;
		m_MapHeight = strlen(s_MapTiles) / s_MapWidth;
		m_TextureMap['G'] = SubTexture2D::CreateFromCoords(m_TileSheet, { 5, 21 }, { 16, 16 });
		m_TextureMap['W'] = SubTexture2D::CreateFromCoords(m_TileSheet, { 8, 17 }, { 16, 16 });
		m_TextureMap['B'] = SubTexture2D::CreateFromCoords(m_TileSheet, { 6, 17 }, { 16, 16 });
		m_TextureMap['T'] = SubTexture2D::CreateFromCoords(m_TileSheet, { 19, 16 }, { 16, 16 });
		m_TextureMap['?'] = SubTexture2D::CreateFromCoords(m_TileSheet, { 37, 8 }, { 16, 16 });

		m_CameraController.SetZoomLevel(5.0f);

		FrameBufferSpecification fbspec;
		fbspec.Width = 1280;
		fbspec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(fbspec);
	}

	void EditorLayer::OnDetach()
	{
		URSA_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(TimeStep ts)
	{
		URSA_PROFILE_FUNCTION();
		//Update
		m_CameraController.OnUpdate(ts);
		//Render
		Renderer2D::ResetStats();

		{
			URSA_PROFILE_SCOPE("Renderer Prep");
			m_FrameBuffer->Bind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();
		}

		{
			static float rotation = 0.0f;
			rotation += ts * 20.0f;
			URSA_PROFILE_SCOPE("Renderer Draw");
			Renderer2D::BeginScene(m_CameraController.GetCamera());
			Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, 0.0f, m_CheckerTexture, { 10.0f, 10.0f }, { 0.1f, 0.1f, 0.1f, 1.0f });

			//Renderer2D::DrawQuad({ -0.5f, 0.0f }, { 0.7f, 0.7f }, glm::radians(rotation), { 0.8f, 0.2f, 0.3f, 1.0f });
			//Renderer2D::DrawQuad({ 0.5f, 0.0f }, { 0.5f, 0.7f }, 0.0f, { 0.4f, 0.8f, 0.8f, 1.0f });
			/*
			for (float y = -5.0f; y < 5.0f; y += 0.5f) {
				for (float x = -5.0f; x < 5.0f; x += 0.5f) {
					glm::vec4 color = { (x + 5.0f) / 10.0f, 0.0f, (y + 5.0f) / 10.0f, 0.5f };
					Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, 0.0f, color);
				}
			}
			*/

			for (uint32_t y = 0; y < m_MapHeight; y++) {
				for (uint32_t x = 0; x < m_MapWidth; x++) {
					char tileType = s_MapTiles[x + y * m_MapWidth];
					Ref<SubTexture2D> texture;
					if (m_TextureMap.find(tileType) != m_TextureMap.end())
						texture = m_TextureMap[tileType];
					else
						texture = m_TextureMap['?'];
					Renderer2D::DrawQuad({ x + 0.5f - m_MapWidth / 2.0f, m_MapHeight - y - 0.5f - m_MapHeight / 2.0f, 0.5f }, { 1.0f, 1.0f }, 0.0f, texture);
				}
			}


			//Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.2f }, { 3.8f, 2.0f }, 0.0f, m_UrsaTitle);
			Renderer2D::EndScene();
			m_FrameBuffer->Unbind();
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		URSA_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("SandBox");
		/*
		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D stats");
		ImGui::Text("Draw calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		*/
		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2(1280.0f, 720.0f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}