#include "Elevator.hpp"
#include "State.hpp"

void Elevator::SetSelectedFloor(uint8_t floor) {
	m_SelectedFloor = floor;
}

void Elevator::ChangeFloor() {
	m_ChangingFloor = true;
	m_PanelAutoClosed = false;
	m_FloorChangingClock.restart();
}

void Elevator::Update(Input& input, State& state) {
	if (m_New) {
		SetSelectedFloor(state.GetLastSelectedLevel());
		m_Panel->SetSelectedFloor(state.GetLastSelectedLevel());
		m_New = false;
	}

	if (m_Panel->IsOpened()) {
		m_Panel->Update(input);

		if (m_Panel->IsSelectionUpdated()) {
			SetSelectedFloor(m_Panel->GetSelectedFloor());
			m_Panel->SetSelectedFloor(m_Panel->GetSelectedFloor());
			ChangeFloor();
			m_ElevatorDoor->SetState(ElevatorDoor::State::Closed);
			m_ElevatorDoor->LockDoors(true);
		}

		if (m_FloorChangingClock.getElapsedTime().asMilliseconds() > 1000 && m_ChangingFloor && !m_PanelAutoClosed) {
			m_PanelAutoClosed = true;
			m_Panel->Close();
		}
	}
	else {
		m_PanelAutoClosed = true;
		m_Engine.Update(input);

		if (m_FloorChangingClock.getElapsedTime().asMilliseconds() > 10000 && m_ChangingFloor) {
			m_ChangingFloor = false;
			m_ElevatorDoor->LockDoors(false);
			m_ElevatorDoor->SetState(ElevatorDoor::State::Opening);
		}

		if (m_ElevatorDoor->PlayerEnter() && !m_ChangingFloor) {
			state.Set((State::States)(m_SelectedFloor + (uint8_t)State::States::Level0));
		}
	}
}

void Elevator::Render(Renderer& renderer) {
	SetCamera(renderer);

	if (m_FloorChangingClock.getElapsedTime().asMilliseconds() > 3000 && m_FloorChangingClock.getElapsedTime().asMilliseconds() < 8000 && m_ChangingFloor) {
		if (m_ShakeClock.getElapsedTime().asMilliseconds() >= 2000) {
			renderer.GetCamera().Shake(sf::milliseconds(std::rand() % 2000 + 500));
			m_ShakeClock.restart();
		}
	}
	else {
		renderer.GetCamera().CancelShake();
	}

	uint64_t index = 0;
	for (std::shared_ptr<Object> obj : m_Engine.m_Objects) {
		if (obj->id == Object::Id::Panel) {
			continue;
		}
		if (index != 0) obj->Render(renderer);
		if (index == 2) {
			float oldPos = m_Player->transform.position.x;
			m_Player->transform.position.x -= 3.0f / 12.0f;
			m_Player->Render(renderer);
			m_Player->transform.position.x = oldPos;
		}
		index++;
	}

	m_Player->Render(renderer);

	m_Player->RenderOverlay(renderer, true);

	m_Panel->Render(renderer);
}

#include "Collision.hpp"

void Elevator::LoadFromFile(const char* filename, Renderer& renderer) {
	AddPlayer(vec2(2.833333f*2, 2.666666f*2));

	InternalLoad(filename, renderer);

	for (std::shared_ptr<Object> obj : m_Engine.m_Objects) {
		if (obj->id == Object::Id::Panel) {
			m_Panel = std::dynamic_pointer_cast<Panel>(obj);
		}
		if (obj->id == Object::Id::ElevatorDoor) {
			m_ElevatorDoor = std::dynamic_pointer_cast<ElevatorDoor>(obj);
		}
	}

	m_Player->transform.position.x = 8 - m_Player->transform.size.w / 2.0f;
	m_Player->transform.position.y = -7.0f / 12.0f - m_Player->transform.size.h;

	FloatRect playerRect = m_Player->transform.rect;

	for (std::shared_ptr<Object>& obj : m_Engine.m_Objects) {
		if (obj->id != Object::Id::CameraView) {
			continue;
		}

		FloatRect collider = obj->colliders[0].box;

		if (RectCollide(playerRect.xy, playerRect.wh, 0, 0, 0, 0, collider.xy, collider.wh, 0, 0, 0, 0)) {
			vec2 target = vec2(m_Player->transform.position.x + m_Player->transform.size.x / 2.0f, m_Player->transform.position.y + m_Player->transform.size.h - 2.833333f + 2.833333f / 3.0f);
			renderer.GetCamera().SetStartRect(obj->transform.rect);
			renderer.GetCamera().SetSize(vec2(16, 9));
			renderer.GetCamera().Target(target);
			renderer.GetCamera().StayInRect(true);
			renderer.GetCamera().Update();
		}
	}
}