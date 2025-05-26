#include "State.hpp"

State::State(Renderer& renderer) : m_Renderer(renderer) {
	Load();
}

uint8_t State::GetLastSelectedLevel() {
	return m_LastSelectedLevel;
}

void State::SetLastSelectedLevel(uint8_t input) {
	m_LastSelectedLevel = input;
}

void State::Set(States state) {
	m_State = state;

	if (state >= States::Level0 && state <= States::Level3) {
		m_LastSelectedLevel = (uint8_t)state - (uint8_t)States::Level0;
	}
}

State::States State::Get() {
	return m_State;
}

void State::OpenPauseMenu() {
	if ((m_Renderer.GetFadeStatus() == Renderer::Fade::InComplete || m_Renderer.GetFadeStatus() == Renderer::Fade::OutComplete)
		&& ((m_State >= States::Level0 && m_State <= States::Level3) || m_State == States::Elevator)) {
		m_PauseMenuOpened = true;
		m_PauseMenu.Reset();
	}
}

void State::ClosePauseMenu() {
	m_PauseMenuOpened = false;
}

void State::Update(Input& input) {
	if (input.IsButtonPressed(Input::Button::Pause)) {
		OpenPauseMenu();
	}

	if (m_PauseMenuOpened) {
		m_PauseMenu.Update(input, *this);
		return;
	}

	if (m_LastState == m_State) {
		m_Current->Update(input, *this);
	}
}

void State::Render() {
	if (m_PauseMenuOpened) {
		m_PauseMenu.Render(m_Renderer);
		return;
	}

	m_Current->Render(m_Renderer);

	if (m_LastState != m_State) {
		if (m_Renderer.GetFadeStatus() == Renderer::Fade::InComplete) {
			m_Renderer.FadeOut(sf::milliseconds(500));
		}
		else if (m_Renderer.GetFadeStatus() == Renderer::Fade::OutComplete) {
			Load();
			m_Renderer.FadeIn(sf::milliseconds(500));
			m_LastState = m_State;
		}
	}
}

void State::Load() {
	m_Renderer.GetTextures().clear();
	m_Current.reset();
	
	switch (m_State) {
	case States::MainMenu:
		m_Current = std::make_shared<MainMenu>();

		break;

	case States::Elevator: {
		std::shared_ptr<Elevator> current = std::make_shared<Elevator>();
		current->LoadFromFile("..\\res\\elevator\\elevator.json", m_Renderer);

		m_Current = current;

		break; 
	}

	case States::Entrance: {
		std::shared_ptr<Entrance> current = std::make_shared<Entrance>();
		current->LoadFromFile("..\\res\\entrance\\level.json", m_Renderer);

		m_Current = current;

		break;
	}

	case States::Level0: {
		std::shared_ptr<Level> current = std::make_shared<Level>();
		current->LoadFromFile("..\\res\\level_0\\level.json", m_Renderer);
		current->SetLevelIndex(0);

		m_Current = current;

		break;
	}

	case States::Level1: {
		std::shared_ptr<Level> current = std::make_shared<Level>();
		current->LoadFromFile("..\\res\\level_1\\level.json", m_Renderer);
		current->SetLevelIndex(1);

		m_Current = current;

		break;
	}

	case States::Level2: {
		std::shared_ptr<Level> current = std::make_shared<Level>();
		current->LoadFromFile("..\\res\\level_2\\level.json", m_Renderer);
		current->SetLevelIndex(2);

		m_Current = current;

		break;
	}

	case States::Level3: {
		std::shared_ptr<Level> current = std::make_shared<Level>();
		current->LoadFromFile("..\\res\\level_3\\level.json", m_Renderer);
		current->SetLevelIndex(3);

		m_Current = current;

		break;
	} 

	case States::Outro:
		m_Current = std::make_shared<Outro>();

		break;

	default: {
		std::shared_ptr<Elevator> current = std::make_shared<Elevator>();
		current->LoadFromFile("..\\res\\elevator\\elevator.json", m_Renderer);

		m_Current = current;

		break; 
	}
	}
}