#include "Door.hpp"
#include "../Player.hpp"

void Door::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::Player) {
		Player& player = *(Player*)other.parent;
		if (player.IsInteracting()) {
			Change();
		}
	}
}

void Door::Change() {
	m_Open = !m_Open;

	if (m_Open) {
		colliders[0].isTrigger = true;

		textureRect.x = textureRect.w;
	}
	else {
		colliders[0].isTrigger = false;

		textureRect.x = 0;
	}
}