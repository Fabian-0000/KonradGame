#include "GlassBox.hpp"

bool GlassBox::IsDestroyed() {
	return m_Destroyed;
}

void GlassBox::OnCollision(const Collider& self, const Collider& other) {
	if (other.parent && other.parent->id == Object::Id::Chandelier) {
		textureRect.x = textureRect.w + 1;
		m_Destroyed = true;
	}
}