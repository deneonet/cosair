#pragma once

#include "crpch.h"
#include "Event.h"

namespace Cosair {

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(double xOffset, double yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) { }

		inline double GetXOffset() const { return m_xOffset; }
		inline double GetYOffset() const { return m_yOffset; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_xOffset << ", " << m_yOffset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		double m_xOffset;
		double m_yOffset;
	};

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(double xPos, double yPos) : m_xPos(xPos), m_yPos(yPos) { }

		inline double GetX() const { return m_xPos; }
		inline double GetY() const { return m_yPos; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_xPos << ", " << m_yPos;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		double m_xPos;
		double m_yPos;
	};

	class MouseButtonEvent : public Event {
	public:
		inline int GetButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput)
	protected:
		MouseButtonEvent(int button) : m_Button(button) { }

		int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) { }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) { }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}