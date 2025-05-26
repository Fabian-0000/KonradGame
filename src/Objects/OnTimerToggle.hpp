#pragma once

#include "../Object.hpp"

class OnTimerToggle : public Object {
public:
	void Update(Input& input) override;

	void Render(Renderer& renderer) override;

	void Toggle();

protected:
	bool m_Exists = true;
	float m_Clock = 0.0f;
};

class OnTimerToggleState0 : public OnTimerToggle {
public:
	OnTimerToggleState0() {
		
	}
};

class OnTimerToggleState1 : public OnTimerToggle {
public:
	OnTimerToggleState1() {
		Toggle();
	}
};