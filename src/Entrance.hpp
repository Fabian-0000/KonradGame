#pragma once

#include "Level.hpp"

class Entrance : public Level {
public:
	Entrance();

	void Update(Input& input, State& state) override;

	void Render(Renderer& renderer);

private:
	Input m_CustomInput;
	Controller m_CustomController;
	Keyboard m_CustomKeyboard;
	Mouse m_CustomMouse;

	float m_Time = 0.0f;
};