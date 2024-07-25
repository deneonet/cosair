#pragma once

#include "Layer.h"
#include "LayerStack.h"
#include "Timestep.h"
#include "Window.h"

#include "Cosair/Core.h"

#include "Cosair/Debug/ImGuiLayer.h"

#include "Cosair/Renderer/Buffer.h"
#include "Cosair/Renderer/Camera.h"
#include "Cosair/Renderer/Shader.h"
#include "Cosair/Renderer/VertexArray.h"

#include "Cosair/Events/Event.h"
#include "Cosair/Events/WindowEvent.h"

#define CR_BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

int main(int argc, char** argv);

namespace Cosair {

	class Application {
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		float m_LastFrameTime = 0;

		bool m_Running = false;
		bool m_Minimized = false;
		std::unique_ptr<Window> m_Window;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();

}

