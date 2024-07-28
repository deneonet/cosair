#include "player_controller.h"

#include "../game.h"

void PlayerController::OnCreate() {
  player_material_ = cosair::CreateRef<cosair::Material>();

  cosair::Texture2dLibrary::Add(
      "Player_Up", cosair::Texture2D::Create(
                       "assets/textures/flappybird/player-upflap.png"));
  cosair::Texture2dLibrary::Add(
      "Player_Mid", cosair::Texture2D::Create(
                        "assets/textures/flappybird/player-midflap.png"));
  cosair::Texture2dLibrary::Add(
      "Player_Down", cosair::Texture2D::Create(
                         "assets/textures/flappybird/player-downflap.png"));

  AddComponent<cosair::Mesh>(player_material_);
}

void PlayerController::OnUpdate(cosair::Timestep ts) {
  if (Game::GetGameState() == GameState::GameOver) {
    velocity_ = 0.0f;
    if (cosair::Input::IsMouseButtonPressed(CR_MOUSE_BUTTON_LEFT))
      Game::OnNewGame();
    return;
  }

  cosair::Transform& transform = GetComponent<cosair::Transform>();

  static float state_start_time = 0;
  state_start_time += ts.GetMilliseconds();

  switch (player_state_) {
    case PlayerState::Idle:
    case PlayerState::Falling:
      if (cosair::Input::IsKeyPressed(CR_KEY_SPACE) &&
          state_start_time >= 100) {
        state_start_time = 0;
        player_state_ = PlayerState::MovingUp;
        velocity_ = kJumpPower;
      }

      // Apply gravity
      velocity_ -= kGravityPower * ts;
      transform.SetPosY(transform.GetPosY() + velocity_ * ts);

      // Ground hit check
      if (transform.GetPosY() <= -0.6f) Game::OnGameOver();
      break;

    case PlayerState::MovingUp:
      velocity_ -= kGravityPower * ts;
      transform.SetPosY(transform.GetPosY() + velocity_ * ts);

      if (velocity_ <= 0) player_state_ = PlayerState::Falling;
      break;
  }

  // Collision Check
  OnCollisionCheck(transform);

  // Fall and Move up rotation
  float target_rot = -50.0f;
  if (velocity_ > -0.4) target_rot = 25.0f;

  float current_rot = transform.GetRotZ();
  float new_rotation =
      current_rot + (target_rot - current_rot) * ts * kRotationSpeed;
  transform.SetRotZ(new_rotation);

  // Player Animation, TODO: Animation System in cosair
  static float last_anim_time = 0;
  static int current_anim_index_ = 0;

  last_anim_time += ts.GetMilliseconds();
  if (last_anim_time > kTimeBetweenEachAnim) {
    if (current_anim_index_ == 3) current_anim_index_ = 0;

    player_material_->SetTexture(cosair::Texture2dLibrary::Load(
        current_anim_index_ == 0   ? "Player_Up"
        : current_anim_index_ == 1 ? "Player_Mid"
                                   : "Player_Down"));

    current_anim_index_++;
    last_anim_time = 0;
  }
}
static cosair::Entity* last_passed_through_pillar = nullptr;

void PlayerController::OnCollisionCheck(cosair::Transform& transform) {
  cosair::Entity* pillars = Game::GetPillars();
  for (int i = 0; i < kPillarsSize; i += 2) {
    cosair::Entity& upper_pillar = pillars[i];
    cosair::Entity& lower_pillar = pillars[i + 1];
    if (!upper_pillar) break;

    cosair::Transform& upper_pillar_transform =
        upper_pillar.GetComponent<cosair::Transform>();
    cosair::Transform& lower_pillar_transform =
        lower_pillar.GetComponent<cosair::Transform>();
    if (upper_pillar_transform.GetPosY() == 5.0f) break;

    float player_pos_x = transform.GetPosX();
    float player_pos_y = transform.GetPosY();

    float upper_pillar_pos_x = upper_pillar_transform.GetPosX();
    float upper_pillar_pos_y = upper_pillar_transform.GetPosY();
    float lower_pillar_pos_x = lower_pillar_transform.GetPosX();
    float lower_pillar_pos_y = lower_pillar_transform.GetPosY();

    static float player_width = transform.GetScaleX();
    static float player_height = transform.GetScaleY();
    static float y_pos_offset = kUpperLowerPillarsYGap + player_height + 0.03;

    bool pass_through_ok =
        last_passed_through_pillar == nullptr ||
        last_passed_through_pillar->GetHandle() != upper_pillar.GetHandle();

    if (player_pos_x + player_width >= upper_pillar_pos_x &&
        player_pos_x <= upper_pillar_pos_x + kPillarsWidth &&
        player_pos_y + y_pos_offset >= upper_pillar_pos_y)
      Game::OnGameOver();
    else if (player_pos_x + player_width >= lower_pillar_pos_x &&
             player_pos_x <= lower_pillar_pos_x + kPillarsWidth &&
             player_pos_y - y_pos_offset <= lower_pillar_pos_y)
      Game::OnGameOver();
    else if (pass_through_ok &&
             player_pos_x > upper_pillar_pos_x + kPillarsWidth &&
             player_pos_x + player_width <
                 upper_pillar_pos_x + 2 * kPillarsWidth &&
             player_pos_y + player_height < upper_pillar_pos_y &&
             player_pos_y > lower_pillar_pos_y) {
      Game::OnPlayerPassThroughGap();
      last_passed_through_pillar = &upper_pillar;
    }
  }
}
