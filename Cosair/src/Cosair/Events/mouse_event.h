#pragma once

#include "event.h"

namespace cosair {

class MouseScrolledEvent : public Event {
 public:
  MouseScrolledEvent(double x_offset, double y_offset)
      : x_offset_(x_offset), y_offset_(y_offset) {}

  inline double GetXOffset() const { return x_offset_; }
  inline double GetYOffset() const { return y_offset_; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseScrolledEvent, X: " << x_offset_ << "; Y: " << y_offset_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(kMouseScrolled)
  EVENT_CLASS_CATEGORY(kEventCategoryMouse | kEventCategoryInput)
 private:
  double x_offset_;
  double y_offset_;
};

class MouseMovedEvent : public Event {
 public:
  MouseMovedEvent(double x_pos, double y_pos) : x_pos_(x_pos), y_pos_(y_pos) {}

  inline double GetX() const { return x_pos_; }
  inline double GetY() const { return y_pos_; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseMovedEvent, X: " << x_pos_ << "; Y: " << y_pos_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(kMouseMoved)
  EVENT_CLASS_CATEGORY(kEventCategoryMouse | kEventCategoryInput)
 private:
  double x_pos_;
  double y_pos_;
};

class MouseButtonEvent : public Event {
 public:
  inline uint32_t GetButton() const { return button_; }

  EVENT_CLASS_CATEGORY(kEventCategoryMouseButton | kEventCategoryInput)
 protected:
  MouseButtonEvent(uint32_t button) : button_(button) {}

 protected:
  uint32_t button_;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
 public:
  MouseButtonPressedEvent(uint32_t button) : MouseButtonEvent(button) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonPressedEvent: " << button_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(kMouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
 public:
  MouseButtonReleasedEvent(uint32_t button) : MouseButtonEvent(button) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonReleasedEvent: " << button_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(kMouseButtonReleased)
};

}  // namespace cosair