#include "crpch.h"
#include "WindowsInput.h"

#include "Cosair/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Cosair {
	
	Scope<Input> Input::s_Instance = std::make_unique<WindowsInput>();

	bool WindowsInput::IsKeyPressed_Impl(int keycode)
	{
		GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressed_Impl(int button)
	{
		GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	void WindowsInput::GetMousePos_Impl(double& xPos, double& yPos)
	{
		GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
		glfwGetCursorPos(window, &xPos, &yPos);
	}

}