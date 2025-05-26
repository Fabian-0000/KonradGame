#pragma once

#include "MainView.hpp"

class Camera {
public:
	void Shake(sf::Time time);

	void CancelShake();

	void StayInRect(bool value);

	void SetRect(FloatRect rect);

	void SetStartRect(FloatRect rect);

	void SetSize(vec2 size);

	void Target(vec2 pos);

	MainView& GetView();

	void DoSmoothTransition();

	void StopSmoothTransition();

	void Update();

private:
	MainView m_View;

	FloatRect m_Rect;

	FloatRect m_OldRect;

	vec2 m_Target;

	vec2 m_Size;

	sf::Clock m_StateChangedClock;

	bool m_Shaking = false;

	sf::Clock m_ShakingClock;

	sf::Time m_ShakingTime;

	bool m_StayInRect = true;

	bool m_SmoothTransition = false;

	bool m_SmoothTransitionCalled = false;
};