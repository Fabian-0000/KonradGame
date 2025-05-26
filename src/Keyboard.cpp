#include "Keyboard.hpp"

#include <cstdlib>
#include <memory>

#ifdef WIN32
#include <Windows.h>
#endif

Keyboard::Keyboard() {
	
}

void Keyboard::EmptyInputList() {
	memset(m_KeysDown, 0, KEY_COUNT);
	memset(m_KeysUp, 0, KEY_COUNT);
}

void Keyboard::Update(sf::Event& evnt) {
	m_IsEvent = false;

	sf::Keyboard::Key code = evnt.key.code < KEY_COUNT ? evnt.key.code : sf::Keyboard::Key::KeyCount;

	if (evnt.type == sf::Event::KeyPressed) {
		m_IsEvent = true;
		m_KeysDown[code] = true;
		m_KeyStates[code] = true;
	}
	if (evnt.type == sf::Event::KeyReleased) {
		m_IsEvent = true;
		m_KeysUp[code] = true;
		m_KeyStates[code] = false;
	}

	m_TextInput = 0;
	if (evnt.type == sf::Event::TextEntered) {
		m_TextInput = evnt.text.unicode;
	}

#ifdef WIN32 
	m_CapsLock = GetKeyState(0x14);
#else 
	m_CapsLock = false;
#endif
}

bool Keyboard::IsKeyDown(sf::Keyboard::Key keycode) const {
	return m_KeyStates[keycode < KEY_COUNT ? keycode : sf::Keyboard::Key::KeyCount];
}

bool Keyboard::IsKeyPressed(sf::Keyboard::Key keycode) const {
	return m_KeysDown[keycode < KEY_COUNT ? keycode : sf::Keyboard::Key::KeyCount];
}

bool Keyboard::IsKeyReleased(sf::Keyboard::Key keycode) const {
	return m_KeysUp[keycode < KEY_COUNT ? keycode : sf::Keyboard::Key::KeyCount];
}

bool Keyboard::IsShiftDown() const {
	bool res = m_KeyStates[sf::Keyboard::Key::LShift] || m_KeyStates[sf::Keyboard::Key::RShift];
	if (m_CapsLock) res = !res;
	return res;
}

bool Keyboard::IsControlDown() const {
	return m_KeyStates[sf::Keyboard::Key::LControl] || m_KeyStates[sf::Keyboard::Key::RControl];
}

bool Keyboard::IsAltDown() const {
	return m_KeyStates[sf::Keyboard::Key::LAlt] || m_KeyStates[sf::Keyboard::Key::RAlt];
}

bool Keyboard::IsAltGrDown() const {
	return (IsAltDown() && IsControlDown()) || IsKeyDown(sf::Keyboard::Key::RAlt);
}

bool Keyboard::IsEvent() {
	return m_IsEvent;
}