#include "Input.hpp"

void Input::Update() {
	if (keyboard->IsEvent() || mouse->IsEvent()) m_LastInputType = InputType::MouseKeyboard;
	else if (controller->IsEvent()) m_LastInputType = InputType::Controller;
}

bool Input::IsButtonDown(Button button) {
	switch (button) {
	case Button::Jump:
		return controller->IsButtonDown(SDL_GAMEPAD_BUTTON_SOUTH) ||
			   keyboard->IsKeyDown(sf::Keyboard::Space);
	case Button::Sprint:
		return controller->IsButtonDown(SDL_GAMEPAD_BUTTON_WEST) ||
			   keyboard->IsControlDown();
	case Button::Dash:
		return controller->GetLeftTriggerState() > 0.5f ||
			   keyboard->IsShiftDown();
	case Button::Sneak:
		return controller->IsButtonDown(SDL_GAMEPAD_BUTTON_LEFT_SHOULDER) ||
			   keyboard->IsKeyDown(sf::Keyboard::S);
	case Button::Interact:
		return controller->IsButtonDown(SDL_GAMEPAD_BUTTON_NORTH) ||
			   keyboard->IsKeyDown(sf::Keyboard::E);
	case Button::Shoot:
		return controller->IsButtonDown(SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER) ||
			   mouse->IsButtonDown(sf::Mouse::Button::Left);
	case Button::DoubleJump:
		return controller->GetRightTriggerState() > 0.5f ||
			   mouse->IsButtonDown(sf::Mouse::Button::Right);
	case Button::Cancel:
		return controller->IsButtonDown(SDL_GAMEPAD_BUTTON_EAST) ||
			   keyboard->IsKeyDown(sf::Keyboard::Escape);
	case Button::Confirm:
		return controller->IsButtonDown(SDL_GAMEPAD_BUTTON_SOUTH) ||
			keyboard->IsKeyDown(sf::Keyboard::Enter);
	case Button::Pause:
		return controller->IsButtonDown(SDL_GAMEPAD_BUTTON_START) ||
			keyboard->IsKeyDown(sf::Keyboard::Escape);
	}
}

bool Input::IsButtonPressed(Button button) {
	switch (button) {
	case Button::Jump:
		return controller->IsButtonPressed(SDL_GAMEPAD_BUTTON_SOUTH) ||
			   keyboard->IsKeyPressed(sf::Keyboard::Space);
	case Button::Sprint:
		return controller->IsButtonPressed(SDL_GAMEPAD_BUTTON_WEST) ||
			   keyboard->IsKeyPressed(sf::Keyboard::LControl) || 
			   keyboard->IsKeyPressed(sf::Keyboard::RControl);
	case Button::Dash:
		return controller->GetLeftTriggerState() > 0.5f ||
			   keyboard->IsKeyPressed(sf::Keyboard::LShift) || 
			   keyboard->IsKeyPressed(sf::Keyboard::RShift);
	case Button::Sneak:
		return controller->IsButtonPressed(SDL_GAMEPAD_BUTTON_LEFT_SHOULDER) ||
			   keyboard->IsKeyPressed(sf::Keyboard::S);
	case Button::Interact:
		return controller->IsButtonPressed(SDL_GAMEPAD_BUTTON_NORTH) ||
			   keyboard->IsKeyPressed(sf::Keyboard::E);
	case Button::Shoot:
		return controller->IsButtonPressed(SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER) ||
			   mouse->IsButtonPressed(sf::Mouse::Button::Left);
	case Button::DoubleJump:
		return controller->GetRightTriggerState() > 0.5f ||
			   mouse->IsButtonPressed(sf::Mouse::Button::Right);
	case Button::Cancel:
		return controller->IsButtonPressed(SDL_GAMEPAD_BUTTON_EAST) ||
			   keyboard->IsKeyPressed(sf::Keyboard::Escape);
	case Button::Confirm:
		return controller->IsButtonPressed(SDL_GAMEPAD_BUTTON_SOUTH) ||
			keyboard->IsKeyPressed(sf::Keyboard::Enter);
	case Button::Pause:
		return controller->IsButtonPressed(SDL_GAMEPAD_BUTTON_START) ||
			keyboard->IsKeyPressed(sf::Keyboard::Escape);
	}
}

vec2 Input::GetDirectionInput() {
	if (controller->getLeftStickMovement().lengthsqr() > 0.0f) {
		m_LastInputType = InputType::Controller;
		return controller->getLeftStickMovement();
	}
	else if (controller->IsButtonDown(SDL_GAMEPAD_BUTTON_DPAD_UP) ||
		controller->IsButtonDown(SDL_GAMEPAD_BUTTON_DPAD_DOWN) ||
		controller->IsButtonDown(SDL_GAMEPAD_BUTTON_DPAD_LEFT) ||
		controller->IsButtonDown(SDL_GAMEPAD_BUTTON_DPAD_RIGHT)) {
		m_LastInputType = InputType::Controller;
		vec2 movement = vec2(
			(float)controller->IsButtonDown(SDL_GAMEPAD_BUTTON_DPAD_RIGHT) - (float)controller->IsButtonDown(SDL_GAMEPAD_BUTTON_DPAD_LEFT),
			(float)controller->IsButtonDown(SDL_GAMEPAD_BUTTON_DPAD_DOWN) - (float)controller->IsButtonDown(SDL_GAMEPAD_BUTTON_DPAD_UP)
		);
		return movement;
	}
	else {
		vec2 movement = {
			(float)keyboard->IsKeyDown(sf::Keyboard::D) - (float)keyboard->IsKeyDown(sf::Keyboard::A),
			(float)keyboard->IsKeyDown(sf::Keyboard::S) - (float)keyboard->IsKeyDown(sf::Keyboard::W) 
		};
		if (movement.lengthsqr() == 0.0f) {
			movement = {
				(float)keyboard->IsKeyDown(sf::Keyboard::Left) - (float)keyboard->IsKeyDown(sf::Keyboard::Right),
				(float)keyboard->IsKeyDown(sf::Keyboard::Down) - (float)keyboard->IsKeyDown(sf::Keyboard::Up)
			};
		}
		if (movement.x != 0 || movement.y != 0) {
			m_LastInputType = InputType::MouseKeyboard;
		}
		return movement;
	}
}

Input::InputType Input::GetLastInputType() {
	return m_LastInputType;
}