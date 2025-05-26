#include "SlotMachine.hpp"

#include "../Player.hpp"
#include "../Particle.hpp"

void SlotMachine::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::Player) {
		if (((Player*)other.parent)->IsInteracting()) {
			std::shared_ptr<Explosion> explosion = std::make_shared<Explosion>();
			explosion->SetSize(vec2(4, 4));

			g_ParticleSystem.Add(explosion, transform.position + transform.size / 2.0f, true);

			Delete();
		}
	}
}