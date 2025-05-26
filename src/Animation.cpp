#include "Animation.hpp"

#include <chrono>

Animation::Animation(uint64_t millis, uint64_t count) {
	m_MilliSeconds = millis;
	m_Count = count;
	m_Over = false;
}

void Animation::Update() {
	if (m_Clock.getElapsedTime().asMilliseconds() >= m_MilliSeconds) {
		m_Clock.restart();
		m_Current++;
		if (m_Current >= m_Count) {
			m_Current = 0;
			m_Over = true;
		}
	}
}

uint64_t Animation::GetIndex() {
	return m_Current;
}

bool Animation::IsOver() {
	return m_Over;
}

void Animation::SetSpeed(uint64_t millis) {
	m_MilliSeconds = millis;
}

void Animation::SetCount(uint64_t count) {
	m_Count = count;
}

void Animation::Reset() {
	m_Current = 0;
	m_Clock.restart();
	m_Over = false;
}