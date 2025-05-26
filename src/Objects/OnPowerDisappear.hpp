#pragma once

#include "../Object.hpp"

class OnPowerDisappear : public Object {
public:
	void Update(Input& input) override;

	void OnCollision(const Collider& self, const Collider& other) override;

	void Render(Renderer& renderer);

private:
	bool m_Powered = false;
};