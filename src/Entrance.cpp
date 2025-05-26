#include "Entrance.hpp"
#include "SaveData.hpp"
#include "State.hpp"

#include "Time.hpp"

Entrance::Entrance() {
	m_CustomInput.controller = &m_CustomController;
	m_CustomInput.keyboard = &m_CustomKeyboard;
	m_CustomInput.mouse = &m_CustomMouse;
}

void Entrance::Update(Input& input, State& state) {
	m_Time += Time.DeltaTime();

	if (m_Time > 5.0f) {
		m_CustomController.m_LeftStick.x = 0.5f;
	}

	m_Engine.Update(m_CustomInput);

	if (m_OutroTrigger.get() && m_OutroTrigger->IsActivated()) {
		state.Set(State::States::Outro);
	}

	else if (m_Level1Warper.get() && m_Level1Warper->IsCollision()) {
		state.Set(State::States::Level0);
		state.SetLastSelectedLevel(0);
	}
}

void Entrance::Render(Renderer& renderer) {
	SetCamera(renderer);

	for (std::shared_ptr<Object> obj : m_Engine.m_Objects) {
		bool render = true;
		for (std::shared_ptr<Darkness>& ptr : m_Darkness) {
			if (ptr.get() == obj.get()) { render = false; break; }
		}
		if (render && obj->exists) obj->Render(renderer);
	}

	m_Player->Render(renderer);

	for (std::shared_ptr<Darkness>& ptr : m_Darkness) {
		ptr->Render(renderer);
	}
}