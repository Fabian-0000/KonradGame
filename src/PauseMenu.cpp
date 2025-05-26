#include "PauseMenu.hpp"
#include "Input.hpp"
#include "Time.hpp"
#include "SaveData.hpp"
#include "State.hpp"

#define PATH "../data/savefile.json"

// !!!Warning!!! Really really bad GUI code (not enough time to optimize)

void PauseMenu::Reset() {
	m_Selected = 0;
	m_SelectionUpdated = false;
	m_Time = 0.0f;
}

void PauseMenu::Update(Input& input, State& state) {
	if (std::abs(input.GetDirectionInput().y) > 0.5f) {
		if (!m_SelectionUpdated) {
			m_Selected += std::roundf(input.GetDirectionInput().y);
		}
		m_SelectionUpdated = true;
	}
	else {
		m_SelectionUpdated = false;
	}

	m_Selected = std::clamp((int)m_Selected, 0, 2);

	m_Time += Time.DeltaTime();

	if (m_Selected == 0 && input.IsButtonPressed(Input::Button::Confirm)) {
		state.ClosePauseMenu();
	}
	else if (m_Selected == 1 && input.IsButtonPressed(Input::Button::Confirm)) {
		state.Set(State::States::MainMenu);
		state.ClosePauseMenu();
	}
	else if (m_Selected == 2 && input.IsButtonPressed(Input::Button::Confirm)) {
		m_Exit = true;
	}

	if (input.IsButtonPressed(Input::Button::Cancel)) {
		state.ClosePauseMenu();
	}
}

void PauseMenu::Render(Renderer& renderer) {
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
		Renderer::MenuTextures::ButtonBack,
		Renderer::MenuTextures::ButtonExit,
	};

	for (uint8_t i = 0; i < 3; i++) {
		sf::RectangleShape object;

		object.setTexture(&renderer.GetMenuTextures()[indices[i]]);
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