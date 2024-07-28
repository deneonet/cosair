#ifdef CR_EXT_NATIVE_SCRIPTING

#pragma once

#include "cosair/scene/scriptable_entity.h"

namespace cosair {

// A cosair component
struct NativeScript {
  ScriptableEntity* instance = nullptr;

  ScriptableEntity* (*instantiate_script)();
  void (*destroy_script)(NativeScript*);

  template <typename T>
  void Bind() {
    instantiate_script = []() {
      return static_cast<ScriptableEntity*>(new T());
    };
    destroy_script = [](NativeScript* native_script) {
      delete native_script->instance;
      native_script->instance = nullptr;
    };
  }
};

}  // namespace cosair

#endif