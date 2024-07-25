#pragma once

#include "crpch.h"

#include "Cosair/Core.h"
#include "Cosair/Events/Event.h"

namespace Cosair {

	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Cosair Engine", unsigned int width = 1280, unsigned int height = 720) : Title(title), Width(width), Height(height) { }
	};

	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		inline bool IsMinimized() const { return m_Minimized; }
		inline void SetMinimized(bool minimized) { m_Minimized = minimized; }

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	protected:
		bool m_Minimized = false;
	};

}