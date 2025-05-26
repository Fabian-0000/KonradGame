#include "MainMenu.hpp"
#include "Input.hpp"
#include "Time.hpp"
#include "SaveData.hpp"
#include "State.hpp"

#define PATH "../data/savefile.json"

// !!!Warning!!! Really really bad GUI code (not enough time to optimize)

MainMenu::MainMenu() {
	m_IsSaveFile = IsSaveData(PATH);
}

void MainMenu::Update(Input& input, State& state) {
	if (std::abs(input.GetDirectionInput().y) > 0.5f) {
		if (!m_SelectionUpdated) {
			m_Selected += std::roundf(input.GetDirectionInput().y);
		}
		m_SelectionUpdated = true;
	}
	else {
		m_SelectionUpdated = false;
	}

	uint8_t maxSelected = 3;
	if (!m_IsSaveFile && !m_DifficultyOpen) maxSelected = 2;

	m_Selected = std::clamp((int)m_Selected, 0, maxSelected-1);

	m_Time += Time.DeltaTime();

	if (!m_DifficultyOpen) {
		if (m_IsSaveFile) {
			if (m_Selected == 2 && input.IsButtonPressed(Input::Button::Confirm)) {
				m_Exit = true;
			}
			else if (m_Selected == 1 && input.IsButtonPressed(Input::Button::Confirm)) {
				m_DifficultyOpen = true;
				m_Selected = 0;
			}
			else if (m_Selected == 0 && input.IsButtonPressed(Input::Button::Confirm)) {
				LoadSaveData(PATH);
				state.Set(State::States::Elevator);
				state.SetLastSelectedLevel(g_SaveData.level);
			}
		}
		else {
			if (m_Selected == 0 && input.IsButtonPressed(Input::Button::Confirm)) {
				m_DifficultyOpen = true;
				m_Selected = 0;
			}
			else if (m_Selected == 1 && input.IsButtonPressed(Input::Button::Confirm)) {
				m_Exit = true;
			}
		}
	}

	else if (m_DifficultyOpen && input.IsButtonPressed(Input::Button::Confirm)) {
		switch (m_Selected) {
		case 0: 
			g_SaveData.hpCount = 0;
			break;

		case 1:
			g_SaveData.hpCount = 5;
			break;

		case 2:
			g_SaveData.hpCount = 1;
			break;
		};

		g_SaveData.hasShotgun = false;
		g_SaveData.level = 0;
		
		state.Set(State::States::Entrance);
		state.SetLastSelectedLevel(0);
	}

	if (input.IsButtonPressed(Input::Button::Cancel) || input.keyboard->IsKeyPressed(sf::Keyboard::Left)) {
		m_DifficultyOpen = false;
		m_Selected = 0;
	}
}

void MainMenu::Render(Renderer& renderer) {
	renderer.GetCamera().SetRect(FloatRect(0, 0, 16, 9));
	renderer.GetCamera().StopSmoothTransition();
	renderer.GetCamera().Update();
	FloatRect r = renderer.GetCamera().GetView().GetRect();

	sf::RectangleShape object;

	object.setTexture(&renderer.GetMenuTextures()[Renderer::MenuTextures::Main]);
	object.setPosition(ToSFMLVec2(r.xy + r.wh / 2.0f));
	object.setSize(ToSFMLVec2(r.wh));
	object.setOrigin(ToSFMLVec2(r.size / 2.0f));

	renderer.GetRenderTarget().draw(object);

	uint8_t indices[] = {
		Renderer::MenuTextures::ButtonContinue,
		Renderer::MenuTextures::ButtonNewGame,
		Renderer::MenuTextures::ButtonExit,
	};

	uint8_t indices2[] = {
		Renderer::MenuTextures::ButtonNewGame,
		Renderer::MenuTextures::ButtonExit,
	};

	uint8_t indices3[] = {
		Renderer::MenuTextures::ButtonEasy,
		Renderer::MenuTextures::ButtonMedium,
		Renderer::MenuTextures::ButtonHard
	};

	uint8_t* usedIndices = indices;
	uint8_t indexCount = 3;

	if (!m_IsSaveFile) { 
		usedIndices = indices2; 
		indexCount = 2;
	}

	if (m_DifficultyOpen) {
		usedIndices = indices3;
		indexCount = 3;
	}

	for (uint8_t i = 0; i < indexCount; i++) {
		sf::RectangleShape object;

		object.setTexture(&renderer.GetMenuTextures()[usedIndices[i]]);
		object.setPosition(ToSFMLVec2(vec2(3.75f, 1.916666f + 1.166666f * i)));
		object.setSize(ToSFMLVec2(vec2(8.5f, 1.083333f)));

		renderer.GetRenderTarget().draw(object);

		if (i == m_Selected && ((int)(m_Time / 0.25f) % 2)) {
			object.setTexture(&renderer.GetMenuTextures()[Renderer::MenuTextures::ButtonOutline]);
			renderer.GetRenderTarget().draw(object);
		}
	}

	if (m_Exit) {
		((sf::RenderWindow&)renderer.GetRenderTarget()).close();
	}
}