#pragma once

namespace Cosair {

	class Timestep {
	public:
		Timestep(float time = 0) : m_Time(time) {}

		operator float() const { return m_Time; }

		inline float GetSeconds() const { return m_Time; }
		inline float GetMilliseconds() const { return m_Time * 1000; }
	private:
		float m_Time;
	};

}