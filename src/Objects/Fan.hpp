#pragma once

#include "../Object.hpp"
#include "../Animation.hpp"

class Player;

class Fan : public Object {
public:
	bool IsActivated();

	void Update(Input& input) override;

	void OnCollision(const Collider& self, const Collider& other) override;

	void Render(Renderer& renderer);

private:
	Animation m_Animation = Animation(200, 2);

	bool m_Collision = false;

	Player* m_Player = nullptr;

	vec2 m_Speed;

	bool m_Activated = true;

	float m_Time = 0.0f;
};