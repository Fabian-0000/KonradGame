#pragma once
#include "OnCollisionLoadLevel1.hpp"
#include "../Player.hpp"

bool OnCollisionLoadLevel1::IsCollision() {
	return m_Collision;
}

void OnCollisionLoadLevel1::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::Player && self.tag == Collider::Tags::Warp) {
		m_Collision = true;
	}
}