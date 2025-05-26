#pragma once

#include "../Object.hpp"

#define MAX_LIGHTS 10

struct LightSource {
	vec2 pos;
	float radius, brightness;
};

class Darkness : public Object {
public:
	Darkness();

	void Update(Input& input) override;

	void OnCollision(const Collider& self, const Collider& other) override;

	void Render(Renderer& renderer) override;

private:
	sf::Shader m_Shader;

	LightSource m_LightSources[MAX_LIGHTS];
	uint8_t m_LightSourcesIndex = 0;

	bool m_Shot = false;
	bool m_ShotLastFrame = false;
	sf::Clock m_ShotClock;

	bool m_FullLight = false;
};