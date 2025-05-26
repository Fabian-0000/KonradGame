#pragma once

#include <SFML/Graphics.hpp>
#include "Renderer.hpp"
#include "BaseState.hpp"

class MainMenu : public BaseState {
public:
	MainMenu();

	void Update(Input& input, State& state) override;

	void Render(Renderer& renderer) override;

private:
	int8_t m_Selected = 0;

	bool m_IsSaveFile = false;

	bool m_SelectionUpdated = false;

	float m_Time = 0.0f;

	bool m_Exit = false;

	bool m_DifficultyOpen = false;
};