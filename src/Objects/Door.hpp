#pragma once
#include "../Object.hpp"

class Door : public Object {
public:
	void OnCollision(const Collider& self, const Collider& other) override;

	void Change();

private:
	bool m_Open = false;
};