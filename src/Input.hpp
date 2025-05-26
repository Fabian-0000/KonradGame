#pragma once

#include "Controller.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"

class Input {
public:
	void Update();

	enum class Button {
		Jump,
		Sprint,
		Dash,
		Sneak,
		Interact,
		Shoot,
		DoubleJump,
		Cancel,
		Confirm,
		Pause
	};

	bool IsButtonDown(Button button);

	bool IsButtonPressed(Button button);

	vec2 GetDirectionInput();

	enum class InputType {
		Controller,
		MouseKeyboard
	};

	InputType GetLastInputType();

	Controller* controller;
	Keyboard* keyboard;
	Mouse* mouse;

private:
	InputType m_LastInputType = InputType::MouseKeyboard;
};