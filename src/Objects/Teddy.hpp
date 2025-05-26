#pragma once
#include "../Object.hpp"

class Teddy : public Object {
public:
	void OnCollision(const Collider& self, const Collider& other) override;
};