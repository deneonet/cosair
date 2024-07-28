#pragma once

#include "cosair/core.h"
#include "layer.h"

namespace cosair {

class LayerStack {
 public:
  ~LayerStack();

  void PushLayer(Layer* layer);
  void PushOverlay(Layer* overlay);
  void PopLayer(Layer* layer);
  void PopOverlay(Layer* overlay);

  std::vector<Layer*>::iterator begin() { return layers_.begin(); }
  std::vector<Layer*>::iterator end() { return layers_.end(); }

 private:
  std::vector<Layer*> layers_;
  unsigned int layer_insert_index_ = 0;
};

}  // namespace cosair