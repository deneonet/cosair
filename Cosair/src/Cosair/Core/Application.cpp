#include "crpch.h"
#include "Application.h"

#include "Input.h"

#include "Cosair/Debug/Log.h"

#include "Cosair/Renderer/Material.h"
#include "Cosair/Renderer/RenderOps.h"
#include "Cosair/Renderer/Renderer2D.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Cosair {
	Application* Application::s_Instance = nullptr;

	// TODO: Ability to set window props when creating the application
	Application::Application() {
		CR_CORE_ASSERT(!s_Instance, "Cannot have more than one application")
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(CR_BIND_EVENT_FN(OnEvent));

		Material::Init();
		RenderOps::Init();
		Renderer2D::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application() {}

	void Application::OnEvent(Event& e) {
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
		if (m_Running) {
			CR_CORE_ERROR("Tried to run Application twice using 'Application::Run', do not run the application manually\n");
			return;
		}
		m_Running = true;

		while (m_Running) {
			// TODO: Make time cross-platform
			float time = (float)glfwGetTime();
			Timestep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			RenderOps::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderOps::Clear();

			if (!m_Minimized) {
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(ts);

			#ifndef CR_DIST
				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			#endif
			}

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PopLayer(Layer* layer) {
		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PopOverlay(Layer* overlay) {
		m_LayerStack.PopOverlay(overlay);
		overlay->OnDetach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		uint32_t width = e.GetWidth();
		uint32_t height = e.GetHeight();
		m_Minimized = width == 0 && height == 0;
		m_Window->SetMinimized(m_Minimized);
		RenderOps::SetViewport(0, 0, width, height);
		return true;
	}

}