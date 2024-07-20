// #define RUN_SANDBOX
#ifdef RUN_SANDBOX
#include "Cosair.h"

#include <imgui/imgui.h>
#include <glm/ext/matrix_transform.hpp>

template<typename Fn>
class Timer {
public:
	Timer(const char* name, Fn&& fn) : m_Name(name), m_Stopped(false), m_Fn(fn) {
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer() {
		if (!m_Stopped)
			Stop();
	}

	void Stop() {
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;
		m_Fn({ m_Name, duration });
	}
private:
	Fn m_Fn;
	bool m_Stopped;
	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

class ExampleLayer : public Cosair::Layer {
public:
	ExampleLayer() : Layer("Example") {}

	void OnAttach() override {
		PROFILE_SCOPE("ExampleLayer::OnAttach");

		Cosair::MaterialRef anyaMaterial = MAKE_MATERIAL_REF();
		anyaMaterial->SetTexture(CREATE_TEXTURE2D("assets/textures/Anya.png"));
		Cosair::MaterialLibrary::Add("Anya", anyaMaterial);

		m_AnyaTransform = MAKE_TRANSFORM_REF(glm::vec3(-0.6f, -0.6f, 0), glm::vec3(0), glm::vec3(0.7f));

		Cosair::MaterialRef luffyMaterial = MAKE_MATERIAL_REF();
		luffyMaterial->SetTexture(CREATE_TEXTURE2D("assets/textures/Luffy.png"));
		Cosair::MaterialLibrary::Add("Luffy", luffyMaterial);

		m_LuffyTransform = MAKE_TRANSFORM_REF();
		m_LuffyTransform->SetScale(glm::vec3(0.7f));
	}

	void OnUpdate(Cosair::Timestep ts) override {
		PROFILE_SCOPE("ExampleLayer::OnUpdate");

		static int framesPerSecond = 0;
		static float lastTime = 0.0f;

		++framesPerSecond;
		lastTime += ts.GetMilliseconds();

		if (lastTime > 1000) {
			m_Fps = framesPerSecond;

			lastTime = 0;
			framesPerSecond = 0;
		}

		{
			PROFILE_SCOPE("Input Polling");

			if (Cosair::Input::IsKeyPressed(CR_KEY_W)) {
				auto pos = m_Camera.GetPosition();
				pos.y += m_CameraMoveSpeed * ts;
				m_Camera.SetPosition(pos);
			}
			else if (Cosair::Input::IsKeyPressed(CR_KEY_S)) {
				auto pos = m_Camera.GetPosition();
				pos.y -= m_CameraMoveSpeed * ts;
				m_Camera.SetPosition(pos);
			}

			if (Cosair::Input::IsKeyPressed(CR_KEY_A)) {
				auto pos = m_Camera.GetPosition();
				pos.x -= m_CameraMoveSpeed * ts;
				m_Camera.SetPosition(pos);
			}
			else if (Cosair::Input::IsKeyPressed(CR_KEY_D)) {
				auto pos = m_Camera.GetPosition();
				pos.x += m_CameraMoveSpeed * ts;
				m_Camera.SetPosition(pos);
			}

			if (Cosair::Input::IsKeyPressed(CR_KEY_J)) {
				m_Camera.SetRotation(
					m_Camera.GetRotation() + m_CameraRotationSpeed * ts
				);
			}
			else if (Cosair::Input::IsKeyPressed(CR_KEY_K)) {
				m_Camera.SetRotation(
					m_Camera.GetRotation() - m_CameraRotationSpeed * ts
				);
			}

			if (Cosair::Input::IsKeyPressed(CR_KEY_I)) {
				auto pos = m_LuffyTransform->GetPosition();
				pos.x -= m_CameraMoveSpeed * ts;
				m_LuffyTransform->SetPosition(pos);
			}
			else if (Cosair::Input::IsKeyPressed(CR_KEY_O)) {
				auto pos = m_LuffyTransform->GetPosition();
				pos.x += m_CameraMoveSpeed * ts;
				m_LuffyTransform->SetPosition(pos);
			}

			if (Cosair::Input::IsKeyPressed(CR_KEY_Y)) {
				auto rot = m_LuffyTransform->GetRotation();
				rot.z += m_CameraRotationSpeed * ts;
				m_LuffyTransform->SetRotation(rot);
			}
			else if (Cosair::Input::IsKeyPressed(CR_KEY_U)) {
				auto rot = m_LuffyTransform->GetRotation();
				rot.z -= m_CameraRotationSpeed * ts;
				m_LuffyTransform->SetRotation(rot);
			}
		}

		{
			PROFILE_SCOPE("Rendering");

			Cosair::Renderer2D::BeginScene(m_Camera);

			Cosair::Renderer2D::DrawQuad("Luffy", m_LuffyTransform);
			Cosair::Renderer2D::DrawQuad("Anya", m_AnyaTransform);

			Cosair::Renderer2D::EndScene();
		}
	}

	void OnImGuiRender() override {
		PROFILE_SCOPE("ExampleLayer::OnImGuiRender");

		ImGui::Begin("Cosair.");

		if (ImGui::CollapsingHeader("Stats")) {
			std::stringstream fps;
			fps << "FPS: " << m_Fps;
			ImGui::Text(fps.str().c_str());

			ImGui::Text("\nProfiling (us = microseconds)\n");
			ImGui::Spacing();

			for (auto& result : m_ProfileResults) {
				char label[50];
				strcpy(label, "%.3fus   ");
				strcat(label, result.Name);
				ImGui::Text(label, result.Time);
			}
		}
		m_ProfileResults.clear();

		if (ImGui::CollapsingHeader("Controls")) {
			ImGui::Text("i = Move Luffy Left      o = Move Luffy Right");
			ImGui::Text("z = Rotate Luffy Left    u = Rotate Luffy Right");
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Text("w = Move Forward         s = Move Backward");
			ImGui::Text("a = Move Left            d = Move Right");
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Text("j = Rotate Left          k = Rotate Right");
		}

		if (ImGui::CollapsingHeader("Settings")) {
			ImGui::Text("VSync: %s", m_VSync ? "Yes" : "No");
			if (ImGui::Button("/\\ Toggle")) {
				m_VSync = !m_VSync;

				Cosair::Window& window = Cosair::Application::Get().GetWindow();
				window.SetVSync(m_VSync);
			}
		}

		ImGui::End();
	}
private:
	int m_Fps = 0;
	bool m_VSync = true;

	float m_CameraMoveSpeed = 3;
	float m_CameraRotationSpeed = 75;

	Cosair::OrthographicCamera m_Camera;

	Cosair::TransformRef m_AnyaTransform;
	Cosair::TransformRef m_LuffyTransform;

	struct ProfileResult {
		const char* Name;
		float Time;
	};
	std::vector<ProfileResult> m_ProfileResults;
	std::vector<ProfileResult> m_ProfileResultsCache;
};

class Sandbox : public Cosair::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Cosair::Application* Cosair::CreateApplication() {
	return new Sandbox();
}
#endif