#include "crpch.h"

#include "Scene.h"

#include "cosair/renderer/renderer_2d.h"
#include "cosair/scene/components/mesh.h"
#include "cosair/scene/components/native_script.h"
#include "cosair/scene/components/transform.h"

namespace cosair {

Scene::Scene() { entt::entity entity = registry_.create(); }
Scene::~Scene() {}

void Scene::Begin() {
  if (!camera_added_) {
    Renderer2D::ResetBatch();
    return;
  }

  Camera& camera = camera_entity_.GetComponent<Camera>();
  Renderer2D::BeginScene(camera.GetViewProjMatrix());
}

void Scene::OnUpdate(Timestep ts, bool dont_end_scene_on_func_end) {
#ifdef CR_EXT_NATIVE_SCRIPTING
  registry_.view<NativeScript>().each(
      [=](entt::entity entt, NativeScript& native_script) {
        if (!native_script.instance) {
          native_script.instance = native_script.instantiate_script();
          native_script.instance->entity_ = Entity{entt, &registry_};
          native_script.instance->OnCreate();
        }
        native_script.instance->OnUpdate(ts);
      });
#endif

  auto group = registry_.group<Transform>(entt::get<Mesh>);
  for (entt::entity entity : group) {
    auto [transform, quad_mesh] = group.get<Transform, Mesh>(entity);
    quad_mesh.Draw(transform);
  }
  if (!dont_end_scene_on_func_end) Renderer2D::EndScene();
}

}  // namespace cosair