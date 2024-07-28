#include <cosair.h>
#include <imgui/imgui.h>

#include "game.h"
#include "scripts/player_controller.h"

class GameLayer : public cosair::Layer {
 public:
  virtual void OnAttach() override {
    scene_ = cosair::CreateRef<cosair::Scene>();

    cosair::Entity camera =
        scene_->CreateCameraEntity(-1.6f, 1.6f, -0.9f, 0.9f);
    scene_->SetMainCamera(camera);

    Game::Init(scene_);
  }

  virtual void OnUpdate(cosair::Timestep ts) override {
    static int framesPerSecond = 0;
    static float lastTime = 0.0f;

    ++framesPerSecond;
    lastTime += ts.GetMilliseconds();

    if (lastTime > 1000) {
      m_Fps = framesPerSecond;

      lastTime = 0;
      framesPerSecond = 0;
    }

    scene_->Begin();
    scene_->OnUpdate(ts);
  }

  virtual void OnImGuiRender() override {
    ImGui::Begin("cosair.");
    std::stringstream fps;
    fps << "FPS: " << m_Fps;
    ImGui::Text(fps.str().c_str());
    ImGui::End();
  }

 private:
  int m_Fps;
  cosair::SceneRef scene_;
};

class FlappyBird : public cosair::Application {
 public:
  FlappyBird() { PushLayer(new GameLayer()); }
  ~FlappyBird() {}
};

#include "../defines.h"
#ifdef PLAY_FLAPPYBIRD
#include <cosair_entry.h>
cosair::Application* cosair::CreateApplication() { return new FlappyBird(); }
#endif