#pragma once
#include "../Object.hpp"

class StaticDestroyable : public Object {
public:
	void OnCollision(const Collider& self, const Collider& other) override;
};