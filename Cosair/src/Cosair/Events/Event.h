#pragma once

#include "Cosair/Core.h"

#include <spdlog/formatter.h>

namespace Cosair {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryWindow = BIT(1),
		EventCategoryInput = BIT(2),
		EventCategoryKeyboard = BIT(3),
		EventCategoryMouse = BIT(4),
		EventCategoryMouseButton = BIT(5),
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
virtual EventType GetEventType() const override { return GetStaticType(); }\
virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event {
		friend class EventDispatcher;
	public:
		bool Handled = false;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		// Debugging Purpose Only
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) const {
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher {
	public:
		EventDispatcher(Event& event) : m_Event(event) { }

		template<typename T, typename F>
		bool Dispatch(const F& func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& event) {
		return os << event.ToString();
	}
}

template<typename T>
struct fmt::formatter<
	T, std::enable_if_t<std::is_base_of<Cosair::Event, T>::value, char>>
	: fmt::formatter<std::string> {
	auto format(const T& event, fmt::format_context& ctx) {
		return fmt::format_to(ctx.out(), "{}", event.ToString());
	}
};