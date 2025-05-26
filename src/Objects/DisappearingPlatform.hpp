#pragma once

#include "../Object.hpp"

class DisappearingPlatform : public Object {
public:
	void Update(Input& input) override;

	void OnCollision(const Collider& self, const Collider& other) override;

	void Render(Renderer& renderer) override;

private:
	sf::Clock m_Timer;
	bool m_Touched = false;
	bool m_Exists = true;
};