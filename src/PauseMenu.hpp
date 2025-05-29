#pragma once

#include <SFML/Graphics.hpp>
#include "Renderer.hpp"
#include "BaseState.hpp"

class PauseMenu : public BaseState {
public:
	void Reset();

	void Update(Input& input, State& state) override;

	void Render(Renderer& renderer) override;

	bool ClosedThisFrame();

private:
	int8_t m_Selected = 0;

	bool m_SelectionUpdated = false;

	float m_Time = 0.0f;

	bool m_Exit = false;

	bool m_OpenedThisFrame = false;

	bool m_ClosedThisFrame = false;
};