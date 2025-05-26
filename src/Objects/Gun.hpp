#pragma once
#include "../Object.hpp"

class Gun : public Object {
public:
	void OnCollision(const Collider& self, const Collider& other) override;

	void Render(Renderer& renderer) override;

private:
	bool m_PickedUp = false;

	bool m_BoxClosed = true;
};