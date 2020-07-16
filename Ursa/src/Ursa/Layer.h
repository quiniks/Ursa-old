#pragma once

#include "Ursa/Core.h"
#include "Ursa/Events/Event.h"

namespace Ursa {
	class URSA_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}
		const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}