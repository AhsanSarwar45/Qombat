#pragma once

#include "Events.hpp"

#include "Input/KeyCodes.hpp"
#include "Utility/Enums.hpp"

namespace QMBT
{
	using namespace Utility;

	// Base (and abstract) class for all key events
	class KeyEvent : public Event
	{
	public:
		inline KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		//Protected constructor to inhibit creation
		KeyEvent(KeyCode keyCode)
			: m_KeyCode(keyCode) {}

		KeyCode m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode keyCode, int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		using RepeatCount = uint32_t;
		inline RepeatCount GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			//Prints in the form: KeyPressedEvent: keyCode (x repeats)
			ss << "KeyPressedEvent: " << EnumToIntegral(m_KeyCode) << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		RepeatCount m_RepeatCount;
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const KeyCode keyCode)
			: KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			//Prints in the form: KeyTypedEvent: keyCode
			ss << "KeyTypedEvent: " << EnumToIntegral(m_KeyCode);
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode keyCode)
			: KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			//Prints in the form: KeyReleasedEvent: keyCode
			ss << "KeyReleasedEvent: " << EnumToIntegral(m_KeyCode);
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}
