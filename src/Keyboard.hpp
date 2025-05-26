#pragma once

#include <SFML/Graphics.hpp>

#define KEY_COUNT sf::Keyboard::Key::KeyCount

class TextInput;

class Keyboard {
public:
	Keyboard();

	void EmptyInputList();

	void Update(sf::Event& evnt);

	// returns the current state of the key
	bool IsKeyDown(sf::Keyboard::Key keycode) const;

	// returns whether the key was pressed in the frame
	bool IsKeyPressed(sf::Keyboard::Key keycode) const;

	// returns whether the key was released in the frame
	bool IsKeyReleased(sf::Keyboard::Key keycode) const;

	bool IsShiftDown() const;

	bool IsControlDown() const;

	bool IsAltDown() const;

	bool IsAltGrDown() const;

	bool IsEvent();

	friend class TextInput;

private:
	bool m_KeysDown[KEY_COUNT + 1] = { false };
	bool m_KeysUp[KEY_COUNT + 1] = { false };
	bool m_KeyStates[KEY_COUNT + 1] = { false };

	char m_TextInput = 0;

	bool m_CapsLock = false;

	bool m_IsEvent = false;
};