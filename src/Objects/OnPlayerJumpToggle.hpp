#pragma once
#include "../Object.hpp"

class Player;

class OnPlayerJumpToggle : public Object {
public:
	void Update(Input& input) override;

	void Render(Renderer& renderer) override;

	void OnCollision(const Collider& self, const Collider& other) override;

	void Toggle();

protected:
	bool m_Exists = true;
	Player* m_Player = nullptr;
};

class OnPlayerJumpToggleState0 : public OnPlayerJumpToggle {
public:
	OnPlayerJumpToggleState0() {
		
	}
};

class OnPlayerJumpToggleState1 : public OnPlayerJumpToggle {
public:
	OnPlayerJumpToggleState1() {
		Toggle();
	}
};