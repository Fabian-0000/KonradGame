#include "Panel.hpp"
#include "../Player.hpp"
#include "../SaveData.hpp"

Panel::Panel() {
	for (uint8_t i = 0; i < std::min(g_SaveData.level + 1, 4); i++) {
		m_ButtonLocked[i] = false;
	}
}

bool Panel::IsOpened() {
	return m_Opened;
}

void Panel::Close() {
	m_Opened = false;
}

bool Panel::IsSelectionUpdated() {
	return m_SelectionConfirmed && !m_ButtonLocked[m_SelectedButton];
}

uint8_t Panel::GetSelectedFloor() {
	return m_SelectedButton;
}

void Panel::SetSelectedFloor(uint8_t floor) {
	m_SelectedButton = std::clamp((int)floor, 0, 3);;
}

void Panel::Update(Input& input) {
	if (!m_Opened) return;

	m_SelectionConfirmed = input.IsButtonPressed(Input::Button::Confirm);

	if (input.IsButtonPressed(Input::Button::Cancel)) {
		m_Opened = false;
	}

	if (abs(input.GetDirectionInput().y) > 0.5f) {
		if (!m_SelectionUpdate) {
			m_SelectedButton += input.GetDirectionInput().y > 0.0f ? -1 : 1;

			m_SelectedButton = std::clamp((int)m_SelectedButton, 0, 3);
		}
		m_SelectionUpdate = true;
	}
	else {
		m_SelectionUpdate = false;
	}
}

void Panel::Render(Renderer& renderer) {
	if (!m_Opened) return;

	sf::RectangleShape object;

	object.setTexture(&renderer.GetElevatorTextures()[Renderer::ElevatorTextures::PanelCloseUp]);
	object.setPosition(sf::Vector2f(0.0f, -9.0f));
	object.setSize(sf::Vector2f(16.0f, 9.0f));

	renderer.GetRenderTarget().draw(object);

	{
		if (m_ButtonLocked[m_SelectedButton])
			object.setTexture(&renderer.GetElevatorTextures()[Renderer::ElevatorTextures::PanelButtonLocked]);
		else
			object.setTexture(&renderer.GetElevatorTextures()[Renderer::ElevatorTextures::PanelButtonSelected]);

		float o = 16.0f / 12.0f * m_SelectedButton;

		if (m_SelectedButton == 3) {
			o = 16.0f / 12.0f * 5;
		}

		object.setPosition(sf::Vector2(0.0f, -9.0f - o));

		renderer.GetRenderTarget().draw(object);
	}
}

void Panel::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::Player && self.tag == Collider::Tags::Interact) {
		Player& player = *(Player*)other.parent;
		if (player.IsInteracting()) {
			m_Opened = true;
		}
	}
}