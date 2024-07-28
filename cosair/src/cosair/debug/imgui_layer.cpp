#include "crpch.h"

#include "imgui_layer.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "cosair/core/application.h"
#include "cosair/core/input.h"

namespace cosair {

bool ImGuiLayer::attached_ = false;

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

#ifdef CR_EXT_IMGUI

void ImGuiLayer::OnAttach() {
  CR_PROFILE_FUNCTION();
  CR_CORE_ASSERT(!attached_,
                 "ImGuiLayer already attached, do not instantiate one: \"new "
                 "ImGuiLayer();\"");
  if (attached_) return;

  attached_ = true;

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  GLFWwindow* window =
      (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach() {
  CR_PROFILE_FUNCTION();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::Begin() {
  CR_PROFILE_FUNCTION();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiLayer::End() {
  CR_PROFILE_FUNCTION();

  ImGuiIO& io = ImGui::GetIO();
  Window& window = Application::Get().GetWindow();
  io.DisplaySize = ImVec2((float)window.GetWidth(), (float)window.GetHeight());

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

#else

void ImGuiLayer::OnAttach() {}
void ImGuiLayer::OnDetach() {}
void ImGuiLayer::Begin() {}
void ImGuiLayer::End() {}

#endif

void ImGuiLayer::OnImGuiRender() {}

}  // namespace cosair