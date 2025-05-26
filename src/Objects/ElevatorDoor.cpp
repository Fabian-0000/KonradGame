#include "ElevatorDoor.hpp"
#include "../Player.hpp"

bool ElevatorDoor::PlayerEnter() {
	return m_PlayerEnter;
}

void ElevatorDoor::LockDoors(bool val) {
	m_DoorsLocked = val;
}

bool ElevatorDoor::DoorsLocked() {
	return m_DoorsLocked;
}

void ElevatorDoor::Update(Input& input) {
	m_AnimationElevatorOpen.Update();

	if (m_DoorsLocked) return;

	if (m_State == State::Opening && m_AnimationElevatorOpen.IsOver()) {
		SetState(State::Open);
	}
	if (m_State == State::Open && m_ElevatorDoorTimer.getElapsedTime().asSeconds() > 8) {
		SetState(State::Closing);
	}
	if (m_State == State::Closing && m_AnimationElevatorOpen.IsOver()) {
		SetState(State::Closed);
	}
}

void ElevatorDoor::Render(Renderer& renderer) {
	size_t elementSize = renderer.GetTextures()[textureIndex].getSize().x / 4;

	if (m_State == State::Opening && !m_AnimationElevatorOpen.IsOver()) {
		textureRect.x = elementSize * m_AnimationElevatorOpen.GetIndex();
	}
	if (m_State == State::Closing && !m_AnimationElevatorOpen.IsOver()) {
		textureRect.x = elementSize * (3 - (m_AnimationElevatorOpen.GetIndex()));
	}
	if (m_State == State::Open) {
		textureRect.x = elementSize * 3;
	}
	if (m_State == State::Closed) {
		textureRect.x = 0;
	}

	sf::RectangleShape object;

	if (hasTexture) {
		object.setTexture(&renderer.GetTextures()[textureIndex]);
		object.setTextureRect(ToSFMLIntRect(textureRect));
	}
	else {
		object.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
	}
	object.setPosition(ToSFMLVec2(transform.position));
	object.setSize(ToSFMLVec2(transform.size));

	renderer.GetRenderTarget().setView(renderer.GetView().GetSFMLView());
	renderer.GetRenderTarget().draw(object);
}

void ElevatorDoor::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::PowerOff) {
		LockDoors(true);
	}
	if (other.tag == Collider::Tags::PowerOn) {
		LockDoors(false);
	}

	if (m_DoorsLocked) return;

	if (other.tag == Collider::Tags::Player && self.tag == Collider::Tags::Interact) {
		Player& player = *(Player*)other.parent;
		if (player.IsInteracting() && m_State == State::Closed) {
			SetState(State::Opening);
		}
		if (player.IsInteracting() && m_State == State::Open) {
			m_PlayerEnter = true;
		}
	}
}

void ElevatorDoor::SetState(State state) {
	switch (state) {
	case State::Closed:
		m_State = State::Closed;
		break;
	case State::Opening:
		m_AnimationElevatorOpen.Reset();
		m_State = State::Opening;
		break;
	case State::Open:
		m_State = State::Open;
		m_ElevatorDoorTimer.restart();
		break;
	case State::Closing:
		m_State = State::Closing;
		m_AnimationElevatorOpen.Reset();
		break;
	}
}