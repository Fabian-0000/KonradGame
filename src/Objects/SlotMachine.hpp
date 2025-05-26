#pragma once
#include "../Object.hpp"

class SlotMachine : public Object {
public:
	void OnCollision(const Collider& self, const Collider& other) override;
};