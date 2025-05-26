#include "OutroTrigger.hpp"
#include "../Player.hpp"

bool OutroTrigger::IsActivated() {
	return m_Activated && m_Clock.getElapsedTime().asMilliseconds() > 1000;
}

void OutroTrigger::OnCollision(const Collider& self, const Collider& other) {
	if (self.tag == Collider::Tags::Interact && other.tag == Collider::Tags::Player) {
		if (((Player*)(other.parent))->IsInteracting()) {
			textureRect.x = textureRect.w;
			m_Activated = true;
			m_Clock.restart();
		}
	}
}