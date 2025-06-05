#include "Mouse.hpp"
#include "View.hpp"

#include <cstring>

void Mouse::EmptyInputList() {
	memset(m_ButtonsDown, 0, MOUSE_BUTTON_COUNT);
	memset(m_ButtonsUp, 0, MOUSE_BUTTON_COUNT);

	m_ScrollDistance = 0;
}

void Mouse::Update(sf::Event& evnt) {
	m_IsEvent = false;

	if (evnt.mouseButton.button >= MOUSE_BUTTON_COUNT || evnt.mouseButton.button < 0) {}

	else if (evnt.type == sf::Event::MouseButtonPressed) {
		m_IsEvent = true;
		m_ButtonsDown[evnt.mouseButton.button] = true;
		m_ButtonStates[evnt.mouseButton.button] = true;
	}
	else if (evnt.type == sf::Event::MouseButtonReleased) {
		m_IsEvent = true;
		m_ButtonsUp[evnt.mouseButton.button] = true;
		m_ButtonStates[evnt.mouseButton.button] = false;
	}

	if (evnt.type == sf::Event::MouseWheelMoved) {
		m_IsEvent = true;
		m_ScrollDistance = evnt.mouseWheel.delta;
	}
}

bool Mouse::IsButtonDown(uint8_t button) {
	if (button >= MOUSE_BUTTON_COUNT) return false;
	return m_ButtonStates[button];
}

bool Mouse::IsAnyButtonDown() {
	for (uint8_t i = 0; i < MOUSE_BUTTON_COUNT; i++) {
		if (IsButtonDown(i)) {
			return true;
		}
	}
	return false;
}

bool Mouse::IsButtonPressed(uint8_t button) {
	if (button >= MOUSE_BUTTON_COUNT) return false;
	return m_ButtonsDown[button];
}

bool Mouse::IsButtonReleased(uint8_t button) {
	if (button >= MOUSE_BUTTON_COUNT) return false;
	return m_ButtonsUp[button];
}

float Mouse::GetScrollDistance() {
	return m_ScrollDistance;
}

vec2 Mouse::GetPosition() {
	return vec2(sf::Mouse::getPosition());
}

vec2 Mouse::GetRelativePosition(const sf::RenderWindow& window) {
	return vec2(sf::Mouse::getPosition(window));
}

vec2 Mouse::GetRelativePositionInView(View& view, const sf::RenderWindow& window) {
	return view.ConvertPixelToCoords((sf::RenderWindow&)window, GetRelativePosition(window));
}

bool Mouse::IsEvent() {
	return m_IsEvent;
}