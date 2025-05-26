#pragma once
#include "../Object.hpp"

class DestroyableWall : public Object {
public:
	void OnCollision(const Collider& self, const Collider& other) override;

private:
	bool m_Destroyed = false;
};