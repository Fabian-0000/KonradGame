#include "StaticDestroyable.hpp"
#include "../Time.hpp"
#include "../Particle.hpp"

void StaticDestroyable::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::PlayerShot && exists) {
		g_ParticleSystem.Add(std::make_shared<Explosion>(), transform.position + transform.size / 2.0f, true);
		exists = false;
	}
}