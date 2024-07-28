#pragma once

#include <spdlog/formatter.h>

#include <sstream>

#include "cosair/core.h"

namespace cosair {

enum class EventType {
  kNone = 0,
  kWindowClose,
  kWindowResize,
  kWindowFocus,
  kWindowLostFocus,
  kWindowMoved,
  kAppTick,
  kAppUpdate,
  kAppRender,
  kKeyPressed,
  kKeyReleased,
  kMouseButtonPressed,
  kMouseButtonReleased,
  kMouseMoved,
  kMouseScrolled
};

enum EventCategory {
  kNone = 0,
  kEventCategoryApplication = BIT(0),
  kEventCategoryWindow = BIT(1),
  kEventCategoryInput = BIT(2),
  kEventCategoryKeyboard = BIT(3),
  kEventCategoryMouse = BIT(4),
  kEventCategoryMouseButton = BIT(5),
};

#define EVENT_CLASS_TYPE(type)                                                \
  static EventType GetStaticType() { return EventType::##type; }              \
  virtual EventType GetEventType() const override { return GetStaticType(); } \
  virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
  virtual int GetCategoryFlags() const override { return category; }

class Event {
  friend class EventDispatcher;

 public:
  virtual EventType GetEventType() const = 0;
  virtual const char* GetName() const = 0;
  virtual int GetCategoryFlags() const = 0;

  // Debugging Purpose Only
  virtual std::string ToString() const { return GetName(); }

  inline bool IsInCategory(EventCategory category) const {
    return GetCategoryFlags() & category;
  }

  inline bool IsHandled() const { return handled_; }
  inline void SetHandled(bool handled) { handled_ = handled; }

 private:
  bool handled_ = false;
};

class EventDispatcher {
 public:
  EventDispatcher(Event& event) : event_(event) {}

  template <typename T, typename F>
  bool Dispatch(const F& func) {
    if (event_.GetEventType() == T::GetStaticType()) {
      event_.SetHandled(func(static_cast<T&>(event_)));
      return true;
    }
    return false;
  }

 private:
  Event& event_;
};

inline std::ostream& operator<<(std::ostream& os, const Event& event) {
  return os << event.ToString();
}

}  // namespace cosair

template <typename T>
struct fmt::formatter<
    T, std::enable_if_t<std::is_base_of<cosair::Event, T>::value, char>>
    : fmt::formatter<std::string> {
  auto format(const T& event, fmt::format_context& ctx) {
    return fmt::format_to(ctx.out(), "{}", event.ToString());
  }
};