#pragma once

#include "../Object.hpp"

class Player;

class MovingPlatformHorizontal : public Object {
public:
	void Update(Input& input) override;

	void OnCollision(const Collider& self, const Collider& other) override;

private:
	float m_Amplitude = 0.0f;
	float m_Center;
	float m_StartAngle = 0.0f;

	Player* m_Player = nullptr;
	bool m_Started = false;

	float m_LaneSize = 0.0f;

	float m_Clock = 0.0f;
	bool m_PlayerTouched = false;
};