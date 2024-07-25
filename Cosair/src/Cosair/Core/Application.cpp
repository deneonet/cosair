#include "crpch.h"
#include "Application.h"

#include "Input.h"

#include "Cosair/Debug/Log.h"

#include "Cosair/Renderer/RenderOps.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Cosair {
	Application* Application::s_Instance = nullptr;

	// TODO: Ability to set window props when creating the application
	Application::Application() {
		CR_PROFILE_FUNCTION();
		CR_CORE_ASSERT(!s_Instance, "Cannot have more than one application")
			s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(CR_BIND_EVENT_FN(OnEvent));

		RenderOps::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application() {
		CR_PROFILE_FUNCTION();
		RenderOps::Shutdown();
	}

	void Application::OnEvent(Event& e) {
		CR_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(CR_BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(CR_BIND_EVENT_FN(OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() {
		CR_PROFILE_FUNCTION();

		if (m_Running) {
			CR_CORE_ERROR("Tried to run Application twice using 'Application::Run', do not run the application manually\n");
			return;
		}
		m_Running = true;

		while (m_Running) {
			CR_PROFILE_SCOPE("Application Run Frame");

			// TODO: Make time cross-platform
			float time = (float)glfwGetTime();
			Timestep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			Cosair::RenderOps::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			Cosair::RenderOps::Clear();

			if (!m_Minimized) {
				{
					CR_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(ts);
				}

			#ifndef CR_DIST
				{
					CR_PROFILE_SCOPE("LayerStack OnImGuiRender");

					m_ImGuiLayer->Begin();
					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
					m_ImGuiLayer->End();
				}
			#endif
			}

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer) {
		CR_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PopLayer(Layer* layer) {
		CR_PROFILE_FUNCTION();
		m_LayerStack.PopLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay) {
		CR_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PopOverlay(Layer* overlay) {
		CR_PROFILE_FUNCTION();
		m_LayerStack.PopOverlay(overlay);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		CR_PROFILE_FUNCTION();

		uint32_t width = e.GetWidth();
		uint32_t height = e.GetHeight();
		m_Minimized = width == 0 && height == 0;
		m_Window->SetMinimized(m_Minimized);
		RenderOps::SetViewport(0, 0, width, height);
		return true;
	}

}