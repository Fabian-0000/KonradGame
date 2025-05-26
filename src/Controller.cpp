#include "Controller.hpp"

#define DEAD_ZONE 4000

bool isInDeadZone(int axisValue) {
	return std::abs(axisValue) < DEAD_ZONE;
}

Controller::Controller() {
	
}

Controller::~Controller() {
	for (SDL_Gamepad* gamepad : m_GamepadList) {
		SDL_CloseGamepad(gamepad);
	}
}

void Controller::EmptyInputList() {
	memset(m_ControllerButtonPressed, 0, SDL_GAMEPAD_BUTTON_COUNT);
}

void Controller::Update() {
	m_IsEvent = false;

	SDL_Event e;
	while (SDL_PollEvent(&e)) {

		switch (e.type) {
		case SDL_EVENT_GAMEPAD_ADDED:
			m_IsEvent = true;
			if (e.gdevice.which) {
				m_GamepadList.push_back(SDL_OpenGamepad(e.gdevice.which));
			}
			break;
		case SDL_EVENT_GAMEPAD_REMOVED:
			if (e.gdevice.which) {
				SDL_Gamepad* gp = nullptr;
				uint64_t index = 0;
				for (SDL_Gamepad* gamepad : m_GamepadList) {
					if (SDL_GetJoystickID(SDL_GetGamepadJoystick(gamepad)) == e.gdevice.which) {
						gp = gamepad;
						break;
					}
					index++;
				}

				SDL_CloseGamepad(gp);
				m_GamepadList.erase(std::next(m_GamepadList.begin(), index));
			}
			break;
		}

		for (SDL_Gamepad* gamepad : m_GamepadList) {
			switch (e.type) {
			case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
				m_IsEvent = true;
				if (gamepad && e.gdevice.which == SDL_GetJoystickID(SDL_GetGamepadJoystick(gamepad))) {
					m_ControllerButtonPressed[e.gbutton.button] = true;
					m_ControllerButtonDown[e.gbutton.button] = true;
				}
				break;
			case SDL_EVENT_GAMEPAD_BUTTON_UP:
				m_IsEvent = true;
				if (gamepad && e.gdevice.which == SDL_GetJoystickID(SDL_GetGamepadJoystick(gamepad))) {
					m_ControllerButtonDown[e.gbutton.button] = false;
				}
				break;
			case SDL_EVENT_GAMEPAD_AXIS_MOTION:
				if (gamepad && e.gdevice.which == SDL_GetJoystickID(SDL_GetGamepadJoystick(gamepad))) {
					int x = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX);
					int y = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY);

					if (isInDeadZone(x) && isInDeadZone(y)) {
						m_LeftStick = 0;
					}
					else { 
						m_IsEvent = true;
						m_LeftStick = vec2(x, y) / 32768.0f; 
					}

					x = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTX);
					y = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTY);

					if (isInDeadZone(x) && isInDeadZone(y)) {
						m_RightStick = 0;
					}
					else { 
						m_IsEvent = true;
						m_RightStick = vec2(x, y) / 32768.0f; 
					}

					if (e.gaxis.axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER) {
						x = e.gaxis.value;
						if (isInDeadZone(x)) {
							m_LeftTrigger = 0.0f;
						}
						else {
							m_IsEvent = true;
							m_LeftTrigger = x / 32768.0f;
						}
					}

					if (e.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) {
						x = e.gaxis.value;
						if (isInDeadZone(x)) {
							m_RightTrigger = 0.0f;
						}
						else {
							m_IsEvent = true;
							m_RightTrigger = x / 32768.0f;
						}
					}
				}
			}
		}
	}
}

bool Controller::IsButtonPressed(SDL_GamepadButton button) {
	return m_ControllerButtonPressed[button];
}

bool Controller::IsButtonDown(SDL_GamepadButton button) {
	return m_ControllerButtonDown[button];
}

bool Controller::IsEvent() {
	return m_IsEvent;
}

float Controller::GetLeftTriggerState() {
	return m_LeftTrigger;
}

float Controller::GetRightTriggerState() {
	return m_RightTrigger;
}

vec2 Controller::getLeftStickMovement() {
	return m_LeftStick;
}

vec2 Controller::getRightStickMovement() {
	return m_RightStick;
}