#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include "Vector2.hpp"

class Entrance;

class Controller {
public:
	Controller();

	~Controller();

	void EmptyInputList();

	void Update();

	bool IsButtonPressed(SDL_GamepadButton button);

	bool IsButtonDown(SDL_GamepadButton button);

	bool IsEvent();

	float GetLeftTriggerState();

	float GetRightTriggerState();

	vec2 getLeftStickMovement();

	vec2 getRightStickMovement();

private:
	std::vector<SDL_Gamepad*> m_GamepadList;

	SDL_Event m_ControllerEvent;

	bool m_ControllerButtonPressed[SDL_GAMEPAD_BUTTON_COUNT] = { false };

	bool m_ControllerButtonDown[SDL_GAMEPAD_BUTTON_COUNT] = { false };

	vec2 m_LeftStick, m_RightStick;

	float m_LeftTrigger = 0.0f, m_RightTrigger = 0.0f;

	bool m_IsEvent = false;

	friend class Entrance;
};