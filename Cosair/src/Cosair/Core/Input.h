#pragma once

namespace Cosair {

	class Input {
	public:
		inline static bool IsKeyPressed(int keycode) {
			return s_Instance->IsKeyPressed_Impl(keycode);
		}

		inline static bool IsMouseButtonPressed(int button) {
			return s_Instance->IsMouseButtonPressed_Impl(button);
		}

		inline static void GetMousePos(double& xPos, double& yPos) {
			return s_Instance->GetMousePos_Impl(xPos, yPos);
		}
	protected:
		virtual bool IsKeyPressed_Impl(int keycode) = 0;
		virtual bool IsMouseButtonPressed_Impl(int button) = 0;
		virtual void GetMousePos_Impl(double& xPos, double& yPos) = 0;
	private:
		static Scope<Input> s_Instance;
	};

}