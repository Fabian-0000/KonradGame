#pragma once
#include "WarpDoor.hpp"

class LockedWarpDoor : public WarpDoor {
public:
	void OnCollision(const Collider& self, const Collider& other) override;

private:
	bool m_Locked = true;
};