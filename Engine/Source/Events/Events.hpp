#pragma once

#include "Core/Core.hpp"

namespace QMBT
{
	enum class EventType : UInt8
	{
		None = 0,
		WindowClosed,
		WindowResized,
		WindowFocused,
		WindowLostFocus,
		WindowMoved,

		AppTicked,
		AppUpdated,
		AppRendered,

		KeyPressed,
		KeyReleased,
		KeyTyped,

		MouseButtonClicked,
		MouseButtonDoubleClicked,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	// For filtering event using a bitfeild
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	//Macro for implementing the following functions in classes deriving from the Event class
#define EVENT_CLASS_TYPE(type)                                                  \
	static EventType GetStaticType() { return EventType::type; }                \
	virtual EventType GetEventType() const override { return GetStaticType(); } \
	virtual const char* GetName() const override { return #type; }

	//Macro for implementing the GetCategoryFlags() function in derived classes
#define EVENT_CLASS_CATEGORY(category) \
	virtual int GetCategoryFlags() const override { return category; }

	//Current event system is blocking, i.e. it is dispatched as soon as it is received.
	//TODO: Buffered event system

	class Event
	{

	  public:
		virtual EventType GetEventType() const = 0;
		virtual int GetCategoryFlags() const = 0;

		//TODO: Only intended to be used for debugging. Enclose in compiler condition.
		virtual const char* GetName() const = 0;
		inline virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }

		//Helps us control how deep down the even will be propagated.
		//If set to true, the layer underneath will not get notified of the event.
		bool IsHandled = false;

	  private:
		friend class EventDispatcher;
	};

	/*
	Allows us to dispatch events based on their type. In our OnEvent function, we will receive
	events as pointers. We do not know the category the Event belongs to. The dispatcher takes
	care of that.
	========

	========	
	*/
	class EventDispatcher
	{

	  public:
		EventDispatcher(Event& event)
			: m_Event(event) {}

		template <typename T, typename Function>
		inline bool Dispatch(const Function& func) // func is a callable that returns true if handled
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.IsHandled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

		OVERLOAD_NEW("Dispatcher");

	  private:
		Event& m_Event;
	};

	// For use by the logging system
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
} // namespace QMBT
