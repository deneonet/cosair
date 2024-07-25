#define RUN_SANDBOX
#ifdef RUN_SANDBOX
#include "Cosair.h"

#include <imgui/imgui.h>

// SANDBOX:
// Just to play around, with bad practices to showcase some features.

// max is 400, because "only" 400 quads are rendered
constexpr static int TEXTURES_TO_RENDER = 400; // + 2

class ExampleLayer : public Cosair::Layer {
public:
	ExampleLayer() : Layer("Example") { }

	void OnAttach() override {
		CR_PROFILE_FUNCTION();

		Cosair::MaterialRef anyaMaterial = MAKE_MATERIAL_REF();
		anyaMaterial->SetTexture(CREATE_TEXTURE2D("assets/textures/Anya.png"));
		Cosair::MaterialLibrary::Add("Anya", anyaMaterial);

		m_AnyaTransform = MAKE_TRANSFORM_REF(glm::vec3(-0.8f, -0.8f, 0), glm::vec3(0), glm::vec3(0.7f));

		m_TextureAtlasMaterial = MAKE_MATERIAL_REF();
		Cosair::TextureAtlasRef textureAtlas = MAKE_TEXTURE_ATLAS_REF("assets/textures/RPGpack_sheet_2X.png");
		textureAtlas->AddSubTexture("Stair", { 7, 6 }, { 128, 128 });
		textureAtlas->AddSubTexture("Fence", { 6, 0 }, { 128, 128 });
		textureAtlas->AddSubTexture("Roof", { 0, 4 }, { 128, 128 }, { 2, 3 });

		m_TextureAtlasMaterial->SetTextureAtlas(textureAtlas);

		// "Stair", "Tree" or "Roof"
		m_TextureAtlasMaterial->SetSubTexture("Stair");

		m_TempMaterial = MAKE_MATERIAL_REF();

		// Bad practice: Use a TextureAtlas instead, but needed to showcase the bindless texture extension.
		int offset = 0;
		for (int i = 0; i < TEXTURES_TO_RENDER; i++) {
			if (offset == 10)
				offset = 0;
			Cosair::Texture2dLibrary::Add(std::to_string(i).c_str(), CREATE_TEXTURE2D("assets/textures/" + std::to_string(offset) + ".png"));
			offset++;
		}
	}

	void OnUpdate(Cosair::Timestep ts) override {
		CR_PROFILE_FUNCTION();

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
			CR_PROFILE_SCOPE("Input Handling");

			if (Cosair::Input::IsKeyPressed(CR_KEY_W))
				m_Camera.SetPositionY(
					m_Camera.GetPositionY() + m_CameraMoveSpeed * ts
				);
			else if (Cosair::Input::IsKeyPressed(CR_KEY_S))
				m_Camera.SetPositionY(
					m_Camera.GetPositionY() - m_CameraMoveSpeed * ts
				);

			if (Cosair::Input::IsKeyPressed(CR_KEY_A))
				m_Camera.SetPositionX(
					m_Camera.GetPositionX() - m_CameraMoveSpeed * ts
				);
			else if (Cosair::Input::IsKeyPressed(CR_KEY_D))
				m_Camera.SetPositionX(
					m_Camera.GetPositionX() + m_CameraMoveSpeed * ts
				);

			if (Cosair::Input::IsKeyPressed(CR_KEY_J))
				m_Camera.SetRotation(
					m_Camera.GetRotation() + m_CameraRotationSpeed * ts
				);
			else if (Cosair::Input::IsKeyPressed(CR_KEY_K))
				m_Camera.SetRotation(
					m_Camera.GetRotation() - m_CameraRotationSpeed * ts
				);

			if (Cosair::Input::IsKeyPressed(CR_KEY_I))
				m_AnyaTransform->SetPositionX(
					m_AnyaTransform->GetPositionX() - m_CameraMoveSpeed * ts
				);
			else if (Cosair::Input::IsKeyPressed(CR_KEY_O))
				m_AnyaTransform->SetPositionX(
					m_AnyaTransform->GetPositionX() + m_CameraMoveSpeed * ts
				);

			if (Cosair::Input::IsKeyPressed(CR_KEY_Y))
				m_AnyaTransform->SetRotationZ(
					m_AnyaTransform->GetRotationZ() + m_CameraRotationSpeed * ts
				);
			else if (Cosair::Input::IsKeyPressed(CR_KEY_U))
				m_AnyaTransform->SetRotationZ(
					m_AnyaTransform->GetRotationZ() - m_CameraRotationSpeed * ts
				);
		}

		{
			CR_PROFILE_SCOPE("Rendering");

			Cosair::Renderer2D::ResetStats();
			Cosair::Renderer2D::BeginScene(m_Camera);

			static Cosair::TransformRef quadsTransform = MAKE_TRANSFORM_REF(glm::vec3(0), glm::vec3(0), glm::vec3(0.45f, 0.45f, 1));

			// Bad practice: Many unnecessary quads and std::to_string <- bad
			int index = 0;
			for (float y = -5.0f; y < 5.0f; y += 0.5f) {
				for (float x = -5.0f; x < 5.0f; x += 0.5f) {
					glm::vec4 color = { (x + 5) / 10, 0.4f, (y + 5) / 10, 1 };
					if (index >= TEXTURES_TO_RENDER)
						index = 0;
					m_TempMaterial->SetColor(color);
					m_TempMaterial->SetTexture(Cosair::Texture2dLibrary::Load(std::to_string(index).c_str()));
					quadsTransform->SetPosition(glm::vec3(x, y, 0));
					Cosair::Renderer2D::DrawQuad(m_TempMaterial, quadsTransform);
					index++;
				}
			}

			Cosair::Renderer2D::DrawQuad(m_TextureAtlasMaterial, MAKE_TRANSFORM_REF());
			Cosair::Renderer2D::DrawQuad("Anya", m_AnyaTransform);

			Cosair::Renderer2D::EndScene();
		}
	}

	void OnImGuiRender() override {
		CR_PROFILE_FUNCTION();

		ImGui::Begin("Cosair.");

		if (ImGui::CollapsingHeader("Stats")) {
			std::stringstream fps;
			fps << "FPS: " << m_Fps;
			ImGui::Text(fps.str().c_str());

			auto& stats = Cosair::Renderer2D::GetStats();

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Text("Quad Count: %d", stats.QuadCount);
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Text("Total Index Count:   %d", stats.GetTotalIndexCount());
			ImGui::Text("Total Vertex Count:  %d", stats.GetTotalVertexCount());
			ImGui::Text("Total Texture Count: %d", TEXTURES_TO_RENDER + 2);
		}

		if (ImGui::CollapsingHeader("Settings")) {
			ImGui::Text("VSync: %s", m_VSync ? "Yes" : "No");
			if (ImGui::Button("/\\ Toggle")) {
				m_VSync = !m_VSync;

				Cosair::Window& window = Cosair::Application::Get().GetWindow();
				window.SetVSync(m_VSync);
			}
			const char* items[] = { "Stair", "Fence", "Roof" };
			static const char* current_item = nullptr;

			if (ImGui::BeginCombo("SubTexture", current_item)) {
				for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
					bool is_selected = (current_item == items[n]);
					if (ImGui::Selectable(items[n], is_selected)) {
						current_item = items[n];
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (current_item) {
				m_TextureAtlasMaterial->SetSubTexture(current_item);
			}
		}

		if (ImGui::CollapsingHeader("Controls")) {
			ImGui::Text("i = Move Anya Left      o = Move Anya Right");
			ImGui::Text("z = Rotate Anya Left    u = Rotate Anya Right");
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Text("w = Move Forward         s = Move Backward");
			ImGui::Text("a = Move Left            d = Move Right");
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Text("j = Rotate Left          k = Rotate Right");
		}

		ImGui::End();
	}
private:
	int m_Fps = 0;
	bool m_VSync = true;

	float m_CameraMoveSpeed = 3;
	float m_CameraRotationSpeed = 75;

	Cosair::OrthographicCamera m_Camera;

	Cosair::MaterialRef m_TempMaterial;
	Cosair::MaterialRef m_TextureAtlasMaterial;

	Cosair::TransformRef m_AnyaTransform;
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