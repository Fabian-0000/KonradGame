#include "LockedWarpDoor.hpp"

#include "../Player.hpp"

void LockedWarpDoor::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::Player && self.tag == Collider::Tags::Interact) {
		m_Player = (Player*)other.parent;

		if (m_Player->IsInteracting()) {
			if (m_Locked && m_Player->UnlockDoor()) {
				m_Locked = false;
			}

			if (!m_Locked) {
				m_Entered = true;
				m_SpriteSwitchTimer.restart();
			}
		}
	}
}