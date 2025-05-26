#include "DestroyableWall.hpp"

#include "../Particle.hpp"

void DestroyableWall::OnCollision(const Collider& self, const Collider& other) {
	if (!self.isTrigger && other.tag == Collider::Tags::PlayerShot && !m_Destroyed) {
		m_Destroyed = true;
		for (Collider& col : colliders) {
			if (col.isTrigger) col.isTrigger = false;
			else col.isTrigger = true;
		}

		textureRect.x = textureRect.w;

		std::shared_ptr<Explosion> explosion = std::make_shared<Explosion>();
		explosion->SetSize(vec2(10, 10));

		g_ParticleSystem.Add(explosion, transform.position + transform.size / 2.0f, true);
	}
}