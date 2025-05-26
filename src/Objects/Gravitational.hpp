#pragma once
#include "../Object.hpp"

class Gravitational : public Object {
public:
	Gravitational();

	void Update(Input& input) override;

	void Render(Renderer& renderer) override;

	void OnCollision(const Collider& self, const Collider& other) override;

private:
	float m_YSpeed = 0.0f;

	bool m_Grounded = true;
};