#include "game.h"

#include "scripts/move_ground.h"
#include "scripts/move_pillar.h"
#include "scripts/player_controller.h"

static std::random_device rd;
static std::mt19937 gen = std::mt19937(rd());
static std::uniform_real_distribution<> dis =
    std::uniform_real_distribution<>(-0.45, 0.35f);

static cosair::Entity player;
static cosair::Entity game_over;

static cosair::Entity score_left;
static cosair::Entity score_right;

static uint32_t pillars_offset = 0;
cosair::Entity Game::pillars[kPillarsSize];
static cosair::Entity grounds[kGroundsSize];

static uint16_t score = 0;
GameState Game::game_state = GameState::Running;

void Game::Init(const cosair::SceneRef& scene) {
  if (game_over) game_over.DestroySelf();

  player = scene->CreateEntity(glm::vec3(0), glm::vec3(0), glm::vec3(0.15f));
  player.AddComponent<cosair::NativeScript>().Bind<PlayerController>();

  cosair::MaterialRef score_material_left =
      cosair::CreateRef<cosair::Material>();
  cosair::MaterialRef score_material_right =
      cosair::CreateRef<cosair::Material>();

  cosair::TextureAtlasRef number_atlas =
      cosair::CreateRef<cosair::TextureAtlas>(
          "assets/textures/flappybird/number-atlas.png");

  number_atlas->AddSubTexture("0", {0, 0}, {24, 36});
  number_atlas->AddSubTexture("1", {9, 0}, {24, 36},
                              {0.6699, 1});  // 16 x 36 px
  number_atlas->AddSubTexture("2", {1, 0}, {24, 36});
  number_atlas->AddSubTexture("3", {2, 0}, {24, 36});
  number_atlas->AddSubTexture("4", {3, 0}, {24, 36});
  number_atlas->AddSubTexture("5", {4, 0}, {24, 36});
  number_atlas->AddSubTexture("6", {5, 0}, {24, 36});
  number_atlas->AddSubTexture("7", {6, 0}, {24, 36});
  number_atlas->AddSubTexture("8", {7, 0}, {24, 36});
  number_atlas->AddSubTexture("9", {8, 0}, {24, 36});

  score_material_left->SetTextureAtlas(number_atlas);
  score_material_left->SetSubTexture("0");

  score_material_right->SetTextureAtlas(number_atlas);
  score_material_right->SetSubTexture("0");

  score_left = scene->CreateEntity(glm::vec3(0, 0.7f, 1.0f), glm::vec3(0),
                                   glm::vec3(0.1f));
  score_left.AddComponent<cosair::Mesh>(score_material_left);

  score_right = scene->CreateEntity(glm::vec3(0.063, 0.7f, -1.0f), glm::vec3(0),
                                    glm::vec3(0.1f));
  score_right.AddComponent<cosair::Mesh>(score_material_right);

  cosair::MaterialRef background_material =
      cosair::CreateRef<cosair::Material>();
  background_material->SetTexture(
      cosair::Texture2D::Create("assets/textures/flappybird/background.png"));

  cosair::Entity background = scene->CreateEntity(
      glm::vec3(0, 0, -0.5f), glm::vec3(0), glm::vec3(3.3f, 2, 3.3f));
  background.AddComponent<cosair::Mesh>(background_material);

  cosair::MaterialRef game_over_material =
      cosair::CreateRef<cosair::Material>();
  game_over_material->SetTexture(
      cosair::Texture2D::Create("assets/textures/flappybird/gameover.png"));
  game_over = scene->CreateEntity(glm::vec3(-0.035, 0, -1.0f), glm::vec3(0),
                                  glm::vec3(1.3f, 0.4f, 1.3f));
  game_over.AddComponent<cosair::Mesh>(game_over_material);

  cosair::MaterialRef ground_material = cosair::CreateRef<cosair::Material>();
  ground_material->SetTexture(
      cosair::Texture2D::Create("assets/textures/flappybird/ground.png"));

  for (int i = 0; i < kGroundsSize; i++) {
    float x = i == 0 ? -0.5f : i == 1 ? 2.0f : 3.5f;
    cosair::Transform ground_transform = {
        glm::vec3(x, -0.77, 1.0f), glm::vec3(0), glm::vec3(2.5f, 0.27f, 2.5f)};

    cosair::Entity ground_entity = scene->CreateEntity(ground_transform);
    ground_entity.AddComponent<cosair::Mesh>(ground_material);
    ground_entity.AddComponent<cosair::NativeScript>().Bind<MoveGround>();
    grounds[i] = ground_entity;
  }

  cosair::MaterialRef pillar_material = cosair::CreateRef<cosair::Material>();
  pillar_material->SetTexture(
      cosair::Texture2D::Create("assets/textures/flappybird/pillar.png"));

  for (int i = 0; i < kPillarsSize; i += 2) {
    float pillar_x = kGapBetweenPillars * i;

    cosair::Entity upper_pillar_entity =
        scene->CreateEntity(glm::vec3(pillar_x, 5, 0));
    upper_pillar_entity.AddComponent<cosair::Mesh>(pillar_material);
    upper_pillar_entity.AddComponent<cosair::NativeScript>().Bind<MovePillar>();

    cosair::Entity lower_pillar_entity =
        scene->CreateEntity(glm::vec3(pillar_x, 5, 0));
    lower_pillar_entity.AddComponent<cosair::Mesh>(pillar_material);
    lower_pillar_entity.AddComponent<cosair::NativeScript>().Bind<MovePillar>();

    pillars[i] = upper_pillar_entity;
    pillars[i + 1] = lower_pillar_entity;
  };
}

void Game::OnNewGame() {
  if (game_state == GameState::Running) return;
  game_state = GameState::Running;

  score = -1;
  // To re-render score
  OnPlayerPassThroughGap();

  score_left.GetTransform().Set(glm::vec3(0, 0.7f, 1.0f), glm::vec3(0),
                                glm::vec3(0.1f));
  score_right.GetTransform().SetPosZ(-1.0f);

  for (int i = 0; i < kPillarsSize; i++)
    pillars[i].GetTransform().SetPosZ(0.5f);
  for (int i = 0; i < kGroundsSize; i++)
    grounds[i].GetTransform().SetPosZ(1.0f);

  game_over.GetTransform().SetPosZ(-1.0f);
  player.GetTransform().SetPos(glm::vec3(0));

  for (int i = 0; i < kGroundsSize; i++) {
    float x = i == 0 ? -0.5f : i == 1 ? 2.0f : 3.5f;
    grounds[i].GetTransform().Set(glm::vec3(x, -0.77, 1.0f), glm::vec3(0),
                                  glm::vec3(2.5f, 0.27f, 2.5f));
  }

  for (int i = 0; i < kPillarsSize; i += 2) {
    float pillar_x = kGapBetweenPillars * i;
    pillars[i].GetTransform().SetPos(glm::vec3(pillar_x, 5, 0));
    pillars[i + 1].GetTransform().SetPos(glm::vec3(pillar_x, 5, 0));
  };

  pillars_offset = 0;
}

void Game::OnGameOver() {
  if (game_state == GameState::GameOver) return;
  game_state = GameState::GameOver;

  for (int i = 0; i < kPillarsSize; i++)
    pillars[i].GetTransform().SetPosZ(-1.0f);
  for (int i = 0; i < kGroundsSize; i++)
    grounds[i].GetTransform().SetPosZ(-1.0f);

  player.GetTransform().SetPosZ(-1.0f);
  game_over.GetTransform().SetPosZ(1.0f);
}

void Game::SetNewRandomPillarPos() {
  if (pillars_offset >= kPillarsSize) pillars_offset = 0;

  float random_height = dis(gen);
  float pillar_y = random_height + .5f;

  cosair::Entity& upper_pillar = pillars[pillars_offset + 0];
  cosair::Entity& lower_pillar = pillars[pillars_offset + 1];
  pillars_offset += 2;

  upper_pillar.GetTransform().Set(
      glm::vec3(1.7f, pillar_y + kUpperLowerPillarsYGap, 0.5f),
      glm::vec3(0, 0, 180), glm::vec3(kPillarsWidth, 1, 1));
  lower_pillar.GetTransform().Set(glm::vec3(1.7f, random_height - .5f, 0.5f),
                                  glm::vec3(0), glm::vec3(kPillarsWidth, 1, 1));
}

void Game::OnPlayerPassThroughGap() {
  std::string score_str = std::to_string(++score);

  cosair::MaterialRef& score_material_left =
      score_left.GetComponent<cosair::Mesh>().GetMaterial();

  if (score_str.length() == 1) {
    score_material_left->SetSubTexture(score_str.c_str());
    return;
  }

  cosair::MaterialRef& score_material_right =
      score_right.GetComponent<cosair::Mesh>().GetMaterial();
  if (score == 10) {
    score_left.GetTransform().Set(glm::vec3(-0.063, 0.7f, 1.0f), glm::vec3(0),
                                  glm::vec3(0.1f));
    score_right.GetTransform().SetPosZ(1.0f);
  }

  if (score_str.length() == 2) {
    score_material_left->SetSubTexture(score_str.substr(0, 1).c_str());
    score_material_right->SetSubTexture(score_str.substr(1, 1).c_str());
    return;
  }

  CR_ERROR("Exceeded score of 99, score is not being updated anymore.");
}