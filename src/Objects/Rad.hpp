#pragma once

#include "../Object.hpp"

class Rad : public Object {
public:
	void Update(Input& input) override;

	void OnCollision(const Collider& self, const Collider& other) override;

	bool IsLocked();

private:
	bool m_IsLocked = false;
};