#pragma once
#include "../Object.hpp"

class Button : public Object {
public:
	void SetAcitvated(bool input);

	void OnCollision(const Collider& self, const Collider& other) override;

	void Update(Input& input) override;

private:
	bool m_Activated = false;
};