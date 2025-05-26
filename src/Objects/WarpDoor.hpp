#pragma once
#include "../Object.hpp"

class Player;

class WarpDoor : public Object {
public:
	virtual void OnCollision(const Collider& self, const Collider& other) override;

	void Render(Renderer& renderer) override;

protected:
	bool m_Entered = false;
	sf::Clock m_SpriteSwitchTimer;
	Player* m_Player = nullptr;
};