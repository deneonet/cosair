#pragma once

#include "Cosair/Core/Layer.h"

namespace Cosair {

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		static bool attached;
	};

}