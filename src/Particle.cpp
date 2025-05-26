#include "Particle.hpp"

void ParticleSystem::Add(std::shared_ptr<Particle> particle, vec2 centerPosition, bool center) {
	if (center) {
		particle->SetPosition(centerPosition - particle->m_Size / 2.0f);
	}
	else {
		particle->SetPosition(centerPosition);
	}
	m_Particles.push_back(particle);
}

void ParticleSystem::Update() {
	for (auto particle : m_Particles) {
		particle->m_Animation.Update();
	}

	bool deleted = false;
	do {
		deleted = false;
		for (uint64_t i = 0; i < m_Particles.size(); i++) {
			if (m_Particles[i]->IsOver()) {
				m_Particles.erase(std::next(m_Particles.begin(), i));
				deleted = true;
				continue;
			}
		}
	} while (deleted);
}

void ParticleSystem::Render(Renderer& renderer) {
	for (auto particle : m_Particles) {
		particle->Render(renderer);
	}
}

void Explosion::Render(Renderer& renderer) {
	sf::RectangleShape object;

	object.setTexture(&renderer.GetParticleTextures()[Renderer::ParticleTextures::Explosion]);

	FloatRect textureRect = {
		32.0f * m_Animation.GetIndex(),
		0.0f,
		32.0f,
		32.0f
	};

	object.setTextureRect(ToSFMLIntRect(textureRect));

	object.setPosition(ToSFMLVec2(m_Position));
	object.setSize(ToSFMLVec2(m_Size));

	renderer.GetRenderTarget().setView(renderer.GetView().GetSFMLView());
	renderer.GetRenderTarget().draw(object);
}

ParticleSystem g_ParticleSystem;