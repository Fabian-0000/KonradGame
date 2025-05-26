#pragma once

#include "View.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"

class MainView : public View {
public:
	void Update(sf::RenderTarget& rt, Mouse& mouse, Keyboard& keyboard);
};