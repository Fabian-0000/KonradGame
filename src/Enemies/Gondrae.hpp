#pragma once

#include "../Object.hpp"
#include "../Animation.hpp"
#include "../Player.hpp"

class Gondrae : public Object {
public:
	void Update(Input& input) override;

	void OnCollision(const Collider& self, const Collider& other) override;

	void Render(Renderer& renderer) override;

private:
	Animation m_AnimationIdle = Animation(300, 5);
	Animation m_AnimationWalk = Animation(200, 6);
	Animation m_AnimationPunch = Animation(100, 3);
	Animation m_AnimationOneFrame = Animation(10000, 1);

	uint8_t m_HP = 3;
	sf::Clock m_HitColorClock;
	bool m_HitRendered = false;

	sf::Clock m_PunchCooldown;

	bool m_Walking = false;
	bool m_Punching = false;

	Player* m_Player = nullptr;

	bool m_Flip = false;

	vec2 m_Speed = 0.0f;

	bool m_Grounded = false;

	sf::Clock m_TurnClock;
};