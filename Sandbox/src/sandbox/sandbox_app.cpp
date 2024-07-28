#include <cosair.h>
#include <imgui/imgui.h>

#include "scripts/anya_controller.h"
#include "scripts/camera_controller.h"

// SANDBOX:
// Just to play around:
// - Materials
// - Transforms
// - Texture2D's
// - Inputs
// - Texture Atlases

// kQuadsDiffTexturesCount = How many different textures to render for all the
// quads?
//
// Max is 400, because "only" 400 quads are rendered
constexpr int kQuadsDiffTexturesCount = 200;  // + 2

class ExampleLayer : public cosair::Layer {
 public:
  ExampleLayer() : Layer("Example") {}

  void OnAttach() override {
    CR_PROFILE_FUNCTION();

    scene_ = cosair::CreateRef<cosair::Scene>();

    cosair::Entity camera_entity =
        scene_->CreateCameraEntity(-1.6f, 1.6f, -0.9f, 0.9f);
    camera_entity.AddComponent<cosair::NativeScript>().Bind<CameraController>();
    scene_->SetMainCamera(camera_entity);

    anya_entity_ = scene_->CreateEntity(glm::vec3(-0.8f, -0.8f, 1),
                                        glm::vec3(0), glm::vec3(0.7f));
    anya_entity_.AddComponent<cosair::NativeScript>().Bind<AnyaController>();

    cosair::MaterialRef anya_material = cosair::CreateRef<cosair::Material>();
    anya_material->SetTexture(
        cosair::Texture2D::Create("assets/textures/sandbox/anya.png"));
    anya_entity_.AddComponent<cosair::Mesh>(cosair::Mesh(anya_material));

    cosair::Entity texture_atlas_entity =
        scene_->CreateEntity(glm::vec3(0, 0, 1));
    texture_atlas_material_ = cosair::CreateRef<cosair::Material>();
    texture_atlas_entity.AddComponent<cosair::Mesh>(
        cosair::Mesh(texture_atlas_material_));

    cosair::TextureAtlasRef texture_atlas =
        cosair::CreateRef<cosair::TextureAtlas>(
            "assets/textures/sandbox/RPGpack_sheet_2X.png");

    texture_atlas->AddSubTexture("Stair", {7, 6}, {128, 128});
    texture_atlas->AddSubTexture("Fence", {6, 0}, {128, 128});
    texture_atlas->AddSubTexture("Roof", {0, 4}, {128, 128}, {2, 3});

    texture_atlas_material_->SetTextureAtlas(texture_atlas);

    // "Stair", "Tree" or "Roof"
    texture_atlas_material_->SetSubTexture("Stair");

    quads_material_ = cosair::CreateRef<cosair::Material>();

    int offset = 0;
    for (int i = 0; i < kQuadsDiffTexturesCount; i++) {
      if (offset == 10) offset = 0;
      quad_textures_[i] = cosair::Texture2D::Create(
          "assets/textures/sandbox/numbers/" + std::to_string(offset) + ".png");
      offset++;
    }
  }

  void OnUpdate(cosair::Timestep ts) override {
    static int framesPerSecond = 0;
    static float lastTime = 0.0f;

    ++framesPerSecond;
    lastTime += ts.GetMilliseconds();

    if (lastTime > 1000) {
      fps_ = framesPerSecond;

      lastTime = 0;
      framesPerSecond = 0;
    }

    cosair::Renderer2D::ResetStats();
    scene_->Begin();

    int i = 0;
    for (float y = -5.0f; y < 5.0f; y += 0.5f) {
      for (float x = -5.0f; x < 5.0f; x += 0.5f) {
        glm::vec4 color = {(x + 5) / 10, 0.4f, (y + 5) / 10, 1};
        if (i >= kQuadsDiffTexturesCount) i = 0;
        quads_material_->SetColor(color);
        quads_material_->SetTexture(quad_textures_[i]);
        quads_transform_.SetPos(glm::vec3(x, y, 0));
        cosair::Renderer2D::DrawQuad(quads_material_, quads_transform_);
        i++;
      }
    }

    scene_->OnUpdate(ts);
  }

  void OnImGuiRender() override {
    ImGui::Begin("cosair.");

    if (ImGui::CollapsingHeader("Stats")) {
      std::stringstream fps;
      fps << "FPS: " << fps_;
      ImGui::Text(fps.str().c_str());

      auto& stats = cosair::Renderer2D::GetStats();

      ImGui::Spacing();
      ImGui::Spacing();
      ImGui::Spacing();

      ImGui::Text("Quad Count: %d", stats.quad_count);
      ImGui::Text("Draw Calls: %d", stats.draw_calls);

      ImGui::Spacing();
      ImGui::Spacing();

      ImGui::Text("Total Index Count:   %d", stats.GetTotalIndexCount());
      ImGui::Text("Total Vertex Count:  %d", stats.GetTotalVertexCount());
      ImGui::Text("Total Texture Count: %d", kQuadsDiffTexturesCount + 2);
    }

    if (ImGui::CollapsingHeader("Settings")) {
      ImGui::Text("VSync: %s", vsync_ ? "Yes" : "No");
      if (ImGui::Button("/\\ Toggle")) {
        vsync_ = !vsync_;

        cosair::Window& window = cosair::Application::Get().GetWindow();
        window.SetVSync(vsync_);
      }
      const char* items[] = {"Stair", "Fence", "Roof"};
      static const char* current_item = nullptr;

      if (ImGui::BeginCombo("SubTexture", current_item)) {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
          bool is_selected = (current_item == items[n]);
          if (ImGui::Selectable(items[n], is_selected)) {
            current_item = items[n];
            if (is_selected) ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }

      if (current_item) {
        texture_atlas_material_->SetSubTexture(current_item);
      }
    }

    if (ImGui::CollapsingHeader("Controls")) {
      ImGui::Text("o = Move Anya Left      p = Move Anya Right");
      ImGui::Text("u = Rotate Anya Left    i = Rotate Anya Right");
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
  int fps_ = 0;
  bool vsync_ = true;

  cosair::SceneRef scene_;

  cosair::Entity anya_entity_;
  cosair::MaterialRef texture_atlas_material_;

  cosair::MaterialRef quads_material_;
  cosair::Texture2dRef quad_textures_[kQuadsDiffTexturesCount];
  cosair::Transform quads_transform_{glm::vec3(0), glm::vec3(0),
                                     glm::vec3(0.45f, 0.45f, 1)};
};

class Sandbox : public cosair::Application {
 public:
  Sandbox() { PushLayer(new ExampleLayer()); }

  ~Sandbox() {}
};

#include "../defines.h"
#ifndef PLAY_FLAPPYBIRD
#include <cosair_entry.h>
cosair::Application* cosair::CreateApplication() { return new Sandbox(); }
#endif