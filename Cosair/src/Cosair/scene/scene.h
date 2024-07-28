#pragma once

#include <entt.hpp>
#include <glm/glm.hpp>

#include "cosair/core/timestep.h"
#include "cosair/scene/components/camera.h"
#include "entity.h"

namespace cosair {

class Entity;

class Scene {
 public:
  Scene();
  ~Scene();

  void Begin();
  void OnUpdate(Timestep ts, bool dont_end_scene_on_func_end = false);

  inline Entity CreateEntity(const Transform& transform) {
    Entity entity = {registry_.create(), &registry_};
    entity.AddComponent<Transform>(transform);
    return entity;
  }

  inline Entity CreateEntity(const glm::vec3& position) {
    Entity entity = {registry_.create(), &registry_};
    entity.AddComponent<Transform>(position);
    return entity;
  }

  inline Entity CreateEntity(const glm::vec3& position,
                             const glm::vec3& rotation) {
    Entity entity = {registry_.create(), &registry_};
    entity.AddComponent<Transform>(position, rotation);
    return entity;
  }

  inline Entity CreateEntity(const glm::vec3& position,
                             const glm::vec3& rotation,
                             const glm::vec3& scale) {
    Entity entity = {registry_.create(), &registry_};
    entity.AddComponent<Transform>(position, rotation, scale);
    return entity;
  }

  // Creates a entity and adds the Camera component to it, but not a Transform
  // component, as the Camera component already has a Transform
  inline Entity CreateCameraEntity(float left, float right, float bottom,
                                   float top, float z_near = -1,
                                   float z_far = 1) {
    Entity entity = {registry_.create(), &registry_};
    entity.AddComponent<Camera>(left, right, bottom, top, z_near, z_far);
    return entity;
  }

  inline void SetMainCamera(Entity& camera_entity) {
    if (!camera_entity.HasComponent<Camera>()) {
      CR_CORE_CRITICAL("Entity has no camera component");
    }

    camera_entity_ = camera_entity;
    camera_added_ = true;
  }

  inline Camera& GetMainCamera() {
    if (!camera_added_) {
      CR_CORE_CRITICAL("No main camera is set");
    }
    return camera_entity_.GetComponent<Camera>();
  }

  inline Entity& GetMainCameraEntity() {
    if (!camera_added_) {
      CR_CORE_CRITICAL("No main camera is set");
    }
    return camera_entity_;
  }

 private:
  Entity camera_entity_;
  bool camera_added_ = false;

  entt::registry registry_;
};

using SceneRef = Ref<Scene>;

}  // namespace cosair