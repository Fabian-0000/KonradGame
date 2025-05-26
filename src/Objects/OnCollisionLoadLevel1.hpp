#pragma once
#include "../Object.hpp"

class OnCollisionLoadLevel1 : public Object {
public:
	bool IsCollision();

	void OnCollision(const Collider& self, const Collider& other) override;

private:
	bool m_Collision = false;
};