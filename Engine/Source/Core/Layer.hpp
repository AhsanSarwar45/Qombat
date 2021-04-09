#pragma once

#include <QMBTPCH.hpp>

#include "Core/Core.hpp"
#include "Core/TimeStep.hpp"
#include "Events/Events.hpp"

namespace QMBT
{
	class Layer
	{
	  public:
		Layer() = delete;
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

		// FUnctionality of the following functions to be extended by derived classes
		inline virtual void OnAttach() {}
		inline virtual void OnDetach() {}
		inline virtual void OnUpdate(const TimeStep& timeStep) {}
		inline virtual void OnImGuiRender() {}
		inline virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	  protected:
		std::string m_DebugName;
	};
} // namespace QMBT
