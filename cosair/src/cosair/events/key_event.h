#pragma once

#include "event.h"

namespace cosair {

class KeyEvent : public Event {
 public:
  inline uint32_t GetKeyCode() const { return keycode_; }

  EVENT_CLASS_CATEGORY(kEventCategoryKeyboard | kEventCategoryInput)
 protected:
  KeyEvent(uint32_t keyCode) : keycode_(keyCode) {}

 protected:
  uint32_t keycode_;
};

class KeyPressedEvent : public KeyEvent {
 public:
  KeyPressedEvent(uint32_t keycode, uint32_t repeat_count)
      : KeyEvent(keycode), repeat_count_(repeat_count) {}

  inline int GetRepeatCount() const { return repeat_count_; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent: " << keycode_ << " (" << repeat_count_
       << " repeats)";
    return ss.str();
  }

  EVENT_CLASS_TYPE(kKeyPressed)
 protected:
  int repeat_count_;
};

class KeyReleasedEvent : public KeyEvent {
 public:
  KeyReleasedEvent(uint32_t keycode) : KeyEvent(keycode) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyReleasedEvent: " << keycode_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(kKeyReleased)
};

}  // namespace cosair