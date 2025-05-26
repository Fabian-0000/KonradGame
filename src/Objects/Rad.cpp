#include "Rad.hpp"
#include "../Player.hpp"

void Rad::Update(Input& input) {
	//m_IsLocked = false;
}

void Rad::OnCollision(const Collider& self, const Collider& other) {
	if (other.parent->id == Object::Id::SlotMachine) {
		m_IsLocked = true;
	}
	if (other.parent->id == Object::Id::Player && !((Player*)other.parent)->IsInteracting()) {
		m_IsLocked = false;
	}
}

bool Rad::IsLocked() {
	return m_IsLocked;
}