#include "Button.hpp"
#include "../Player.hpp"

void Button::SetAcitvated(bool input) {
	m_Activated = input;
}

void Button::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::Player && self.tag == Collider::Tags::Interact) {
		Player& player = *(Player*)other.parent;
		if (player.IsInteracting()) {
			m_Activated = !m_Activated;

			textureRect.x = m_Activated ? textureRect.w : 0;

			for (Collider& col : colliders) {
				if (col.tag == Collider::Tags::PowerOff || col.tag == Collider::Tags::PowerOn) {
					if (col.tag == Collider::Tags::PowerOff) col.tag = Collider::Tags::PowerOn;
					else if (col.tag == Collider::Tags::PowerOn) col.tag = Collider::Tags::PowerOff;
				}
			}
		}
	}
}

void Button::Update(Input& input) {
	textureRect.x = m_Activated ? textureRect.w : 0;
}