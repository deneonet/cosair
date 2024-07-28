#ifdef CR_EXT_NATIVE_SCRIPTING

#pragma once

#include "cosair/core/timestep.h"
#include "entity.h"
#include "scene.h"

namespace cosair {

class ScriptableEntity {
 public:
  virtual ~ScriptableEntity() {}

  inline void DestroySelf() { entity_.DestroySelf(); }

  template <typename T>
  inline bool HasComponent() {
    return entity_.HasComponent<T>();
  }

  template <typename T>
  inline void RemoveComponent() {
    entity_.RemoveComponent<T>();
  }

  template <typename T>
  inline T& GetComponent() {
    return entity_.GetComponent<T>();
  }

  template <typename T, typename... Args>
  inline T& AddComponent(Args&&... args) {
    return entity_.AddComponent<T>(std::forward<Args>(args)...);
  }

  // GetTransform() is a short form of GetComponent<Transform>() and is not
  // available for camera entities
  inline Transform& GetTransform() { return entity_.GetTransform(); }

  // TODO: This function does currently nothing.
  inline void SetActive(bool active) { entity_.SetActive(active); }
  inline bool IsActive() const { return entity_.IsActive(); }

 protected:
  virtual void OnCreate() {}
  virtual void OnDestroy() {}
  virtual void OnUpdate(Timestep ts) {}

 private:
  Entity entity_;
  friend class Scene;
};

}  // namespace cosair

#endif