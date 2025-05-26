#pragma once

#include <SFML/Graphics.hpp>

#include "Vector2.hpp"

#pragma once

#define MOUSE_BUTTON_COUNT 3

class View;

class Mouse {
public:
	void EmptyInputList();

	void Update(sf::Event& evnt);

	// returns the current state of the key
	bool IsButtonDown(uint8_t button);

	bool IsAnyButtonDown();

	// returns whether the key was pressed in the frame
	bool IsButtonPressed(uint8_t button);

	// returns whether the key was released in the frame
	bool IsButtonReleased(uint8_t button);

	// returns a positive value for up and a negative value for down
	float GetScrollDistance();

	vec2 GetPosition();

	vec2 GetRelativePosition(const sf::RenderWindow& window);

	vec2 GetRelativePositionInView(View& view, const sf::RenderWindow& window);

	bool IsEvent();

private:
	bool m_ButtonsDown[MOUSE_BUTTON_COUNT] = { false };
	bool m_ButtonsUp[MOUSE_BUTTON_COUNT] = { false };
	bool m_ButtonStates[MOUSE_BUTTON_COUNT] = { false };

	int8_t m_ScrollDistance = 0.0f;

	bool m_IsEvent = false;
};