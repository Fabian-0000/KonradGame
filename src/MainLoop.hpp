#pragma once

#include <SFML/Graphics.hpp>
#include "Renderer.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"

class MainLoop {
public:
	MainLoop();

	void Run();

private:
	sf::RenderWindow m_Window;
	Renderer m_Renderer;

	Mouse m_Mouse;
	Keyboard m_Keyboard;
};