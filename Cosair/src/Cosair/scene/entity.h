#pragma once

#include <entt.hpp>

#include "cosair/debug/log.h"

namespace cosair {

namespace entity {

inline constexpr entt::null_t null{};

}

class Entity {
 public:
  Entity() = default;
  Entity(entt::entity handle, entt::registry* registry)
      : handle_(handle), registry_(registry) {}

  inline void DestroySelf() {
    registry_->destroy(handle_);
    handle_ = entt::null;
  }

  template <typename T>
  inline bool HasComponent() {
    return registry_->all_of<T>(handle_);
  }

  template <typename T>
  inline void RemoveComponent() {
    registry_->remove<T>(handle_);
  }

  template <typename T, typename... Args>
  inline T& AddComponent(Args&&... args) {
    if (HasComponent<T>()) {
      CR_CORE_CRITICAL(
          "Entity already has that component, check first using "
          "'Entity::HasComponent'");
    }
    return registry_->emplace<T>(handle_, std::forward<Args>(args)...);
  }

  template <typename T>
  inline T& GetComponent() {
    if (!HasComponent<T>()) {
      CR_CORE_CRITICAL(
          "Entity doesn't have that component, check first using "
          "'Entity::HasComponent'");
    }
    return registry_->get<T>(handle_);
  }

  // GetTransform() is a short form of GetComponent<Transform>() and is not
  // available for camera entities
  inline Transform& GetTransform() { return GetComponent<Transform>(); }

  // TODO: Swap entity into a registry that contains all inactive entities and
  // vice-versa. Short: This function does currently nothing.
  inline void SetActive(bool active) { active_ = active; }
  inline bool IsActive() const { return active_; }

  inline entt::entity GetHandle() const { return handle_; }

  operator bool() const { return handle_ != entt::null; }

 private:
  // TODO: Move into a std::weak_ptr
  entt::registry* registry_ = nullptr;
  entt::entity handle_ = entt::null;

  bool active_ = true;
};

}  // namespace cosair