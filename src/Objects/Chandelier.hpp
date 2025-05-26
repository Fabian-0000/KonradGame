#pragma once
#include "../Object.hpp"

class Chandelier : public Object {
public:
	void Update(Input& input) override;

	void Render(Renderer& renderer) override;

	void OnCollision(const Collider& self, const Collider& other) override;

private:
	bool m_Exists = true;

	bool m_Falling = false;

	float m_YSpeed = 0.0f;
};