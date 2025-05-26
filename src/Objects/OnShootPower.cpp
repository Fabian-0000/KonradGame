#include "OnShootPower.hpp"
#include "../Player.hpp"

bool OnShootPower::IsActivated() {
	return m_Activated;
}

void OnShootPower::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::PlayerShot && !m_Activated) {
		m_Activated = true;

		for (Collider& col : colliders) {
			if (col.tag == Collider::Tags::PowerOff || col.tag == Collider::Tags::PowerOn) {
				if (col.tag == Collider::Tags::PowerOff) col.tag = Collider::Tags::PowerOn;
				else if (col.tag == Collider::Tags::PowerOn) col.tag = Collider::Tags::PowerOff;
			}
		}
	}
}

void OnShootPower::Render(Renderer& renderer) {
	
}