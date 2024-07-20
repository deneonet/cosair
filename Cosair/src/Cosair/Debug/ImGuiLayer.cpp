#include "crpch.h"
#include "ImGuiLayer.h"

#include "Cosair/Core/Input.h"
#include "Cosair/Core/Application.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Cosair {

	bool ImGuiLayer::attached = false;

	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

#ifdef CR_DIST
	void ImGuiLayer::OnAttach() {}
	void ImGuiLayer::OnDetach() {}
	void ImGuiLayer::Begin() {}
	void ImGuiLayer::End() {}
#else
	void ImGuiLayer::OnAttach() {
		CR_CORE_ASSERT(!attached, "ImGuiLayer already attached, do not instantiate one: \"new ImGuiLayer();\"");
		if (attached) return;

		attached = true;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End() {
		ImGuiIO& io = ImGui::GetIO();
		Window& window = Application::Get().GetWindow();
		io.DisplaySize = ImVec2((float)window.GetWidth(), (float)window.GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
#endif

	void ImGuiLayer::OnImGuiRender() {}
}