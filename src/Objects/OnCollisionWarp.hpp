#pragma once
#include "../Object.hpp"

class Player;

class OnCollisionWarp : public Object {
public:
	void OnCollision(const Collider& self, const Collider& other) override;

	void Render(Renderer& renderer) override;

private:
	bool m_Entered = false;
	Player* m_Player = nullptr;
};