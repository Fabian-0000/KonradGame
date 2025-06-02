#pragma once

#include "BaseState.hpp"
#include "Level.hpp"
#include "Elevator.hpp"
#include "Outro.hpp"
#include "MainMenu.hpp"
#include "Entrance.hpp"
#include "PauseMenu.hpp"

#include <memory>

class State {
public:
	State(Renderer& renderer);

	enum class States {
		MainMenu,
		PauseMenu,
		Elevator,
		Entrance,
		Level0,
		Level1,
		Level2,
		Level3,
		Outro
	};

	void Set(States state);

	States Get();

	void Update(Input& input);

	void Render();

	uint8_t GetLastSelectedLevel();

	void SetLastSelectedLevel(uint8_t input);

	void OpenPauseMenu();

	void ClosePauseMenu();

	bool PauseMenuOpen();

private:
	void Load();

	States m_State = States::MainMenu;

	States m_LastState = States::MainMenu;

	std::shared_ptr<BaseState> m_Current;

	PauseMenu m_PauseMenu;

	bool m_PauseMenuOpened = false;

	Renderer& m_Renderer;

	uint8_t m_LastSelectedLevel = 0;
};