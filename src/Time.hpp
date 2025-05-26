#pragma once

#include <SFML/System.hpp>

class DeltaClock {
public:
	float DeltaTime() {
		return m_DeltaTime;
	}

	void UpdateDeltaTime() {
		m_DeltaTime = m_Clock.restart().asSeconds();
		if (m_DeltaTime > 0.02f) m_DeltaTime = 0.02f;
	}

private:
	sf::Clock m_Clock;

	float m_DeltaTime = 0.0f;
};

extern DeltaClock Time;