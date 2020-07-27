#pragma once

#include "Ursa/Layer.h"
#include "Ursa/Events/ApplicationEvent.h"
#include "Ursa/Events/KeyEvent.h"
#include "Ursa/Events/MouseEvent.h"

namespace Ursa {
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}