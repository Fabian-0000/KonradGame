#pragma once

#include "../Object.hpp"

class OutroTrigger : public Object {
public:
	void OnCollision(const Collider& self, const Collider& other) override;

	bool IsActivated();

private:
	bool m_Activated;
	sf::Clock m_Clock;
};