#pragma once

namespace QMBT
{
	class TimeStep
	{
	  public:
		explicit TimeStep(float time)
			: m_Time(time) {}

		operator float() const { return m_Time; }
		float GetSeconds() const { return m_Time; }
		float GetMilliSeconds() const { return m_Time * 1000.0f; }

	  private:
		float m_Time;
	};
} // namespace QMBT
