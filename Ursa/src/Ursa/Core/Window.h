#pragma once

#include "ursapch.h"
#include "Ursa/Core/Core.h"
#include "Ursa/Events/Event.h"

namespace Ursa {
	struct WindowProperties {
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProperties(
			const std::string& title = "Ursa",
			uint32_t width = 1280,
			uint32_t height = 720
		) : Title(title), Width(width), Height(height) {}
	};

	//Interface representing a desktop system based window
	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;
		virtual void OnUpdate() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProperties& props = WindowProperties());
	};
}