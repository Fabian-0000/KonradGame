#pragma once
#include "../Object.hpp"

class OnShootPower : public Object {
public:
	bool IsActivated();

	void OnCollision(const Collider& self, const Collider& other) override;

	void Render(Renderer& renderer) override;

private:
	bool m_Activated = false;
};