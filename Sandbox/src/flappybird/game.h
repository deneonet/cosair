#pragma once

#include <cosair.h>

#include <random>

enum class GameState { Running, GameOver };

// Upper + Lower Pillars Size,
// so 6 upper + lower pillars are 3 complete pillars
//
// Has to be divisable by 2
constexpr size_t kPillarsSize = 6;
// X gap between each complete pillar
// Going Lower = Pillars will be closer together
constexpr float kGapBetweenPillars = 0.75f;
// Y gap between the lower and upper pillar
constexpr float kUpperLowerPillarsYGap = 0.37f;
// Width of each complete pillar
constexpr float kPillarsWidth = 0.25f;
// Shouldn't be changed
constexpr size_t kGroundsSize = 3;

class Game {
 public:
  static void Init(const cosair::SceneRef& scene);

  static void OnNewGame();
  static void OnGameOver();

  static void SetNewRandomPillarPos();
  static void OnPlayerPassThroughGap();

  static inline GameState GetGameState() { return game_state; }
  inline static cosair::Entity* GetPillars() { return pillars; }

 private:
  static GameState game_state;
  static cosair::Entity pillars[kPillarsSize];
};