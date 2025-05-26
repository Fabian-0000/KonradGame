#pragma once

#include "Renderer.hpp"
#include "Animation.hpp"

class ParticleSystem;

class Particle {
public:
	Particle() {
		m_Animation.SetSpeed(100);
		m_Animation.SetCount(7);
	}

	bool IsOver() {
		return m_Animation.IsOver();
	}

	virtual void SetPosition(vec2 pos) {
		m_Position = pos;
	}

	void SetSize(vec2 size) {
		m_Size = size;
	}

	virtual void Render(Renderer& renderer) {}

protected:
	friend class ParticleSystem;

	vec2 m_Position;

	vec2 m_Size = vec2(2.666666f, 2.666666f);

	Animation m_Animation;
};

class Explosion : public Particle {
public:
	void Render(Renderer& renderer) override;
};

class ParticleSystem {
public:
	void Add(std::shared_ptr<Particle> particle, vec2 position, bool center = false);

	void Update();

	void Render(Renderer& renderer);

private:
	std::vector<std::shared_ptr<Particle>> m_Particles;
};

extern ParticleSystem g_ParticleSystem;