#ifdef CR_PLATFORM_WINDOWS

#pragma once

#include "Cosair/Core/Window.h"
#include "Cosair/Renderer/RenderingContext.h"

#include <GLFW/glfw3.h>

struct GLFWwindow;

namespace Cosair {

	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void* GetNativeWindow() const override { return m_Window; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		RenderingContext* m_RenderingContext;

		struct WindowData {
			std::string Title = "";
			unsigned int Width = 0;
			unsigned int Height = 0;

			bool VSync = true;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}

#endif