#pragma once

#include "Cosair/Core/Input.h"

namespace Cosair {

	class WindowsInput : public Input {
	protected:
		virtual bool IsKeyPressed_Impl(int keycode) override;
		virtual bool IsMouseButtonPressed_Impl(int button) override;
		virtual void GetMousePos_Impl(double& xPos, double& yPos) override;
	};

}