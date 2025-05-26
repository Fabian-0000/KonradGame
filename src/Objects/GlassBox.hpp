#pragma once
#include "../Object.hpp"

class GlassBox : public Object {
public:
	bool IsDestroyed();

	void OnCollision(const Collider& self, const Collider& other) override;

private:
	bool m_Destroyed = false;
};