#pragma once

#include "SFML/Graphics.hpp"
#include "BaseState.hpp"

class Outro : public BaseState {
public:
	void Update(Input& input, State& state) override;

	void Render(Renderer& renderer) override;

private:
	sf::Clock m_AnimationClock;

	bool m_Explosion = false;

	bool m_Fading = false;
};