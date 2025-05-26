#pragma once
#include <cstdint>
#include <SFML/System.hpp>

class Animation {
public:
	Animation() {}

	Animation(uint64_t millis, uint64_t count);

	void Update();

	uint64_t GetIndex();

	void SetSpeed(uint64_t millis);

	void SetCount(uint64_t millis);

	bool IsOver();

	void Reset();

private:
	uint64_t m_MilliSeconds = 0;
	uint64_t m_Current = 0;
	uint64_t m_Count = 0;
	sf::Clock m_Clock;
	bool m_Over = false;
};