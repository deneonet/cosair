#pragma once

#include "event.h"

namespace cosair {

class WindowCloseEvent : public Event {
 public:
  WindowCloseEvent() {}

  EVENT_CLASS_TYPE(kWindowClose)
  EVENT_CLASS_CATEGORY(kEventCategoryWindow)
};

class WindowResizeEvent : public Event {
 public:
  WindowResizeEvent(uint32_t width, uint32_t height)
      : width_(width), height_(height) {}

  inline uint32_t GetWidth() const { return width_; }
  inline uint32_t GetHeight() const { return height_; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowResizeEvent, Width: " << width_ << "; Height: " << height_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(kWindowResize)
  EVENT_CLASS_CATEGORY(kEventCategoryWindow)
 protected:
  int width_;
  int height_;
};

}  // namespace cosair