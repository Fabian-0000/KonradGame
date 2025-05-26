#include "Teddy.hpp"
#include "../Particle.hpp"

void Teddy::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::PlayerShot && self.tag == Collider::Tags::Damage) {
		g_ParticleSystem.Add(std::make_shared<Explosion>(), transform.position + transform.size / 2.0f, true);
		exists = false;
	}
}