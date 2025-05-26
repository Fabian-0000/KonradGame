#include "Outro.hpp"
#include "Particle.hpp"
#include "Input.hpp"
#include "State.hpp"

void Outro::Update(Input& input, State& state) {
	if (m_AnimationClock.getElapsedTime().asSeconds() > 10) {
		if (input.IsButtonPressed(Input::Button::Confirm) || input.IsButtonPressed(Input::Button::Cancel)) {
			state.Set(State::States::MainMenu);
		}
	}
}

void Outro::Render(Renderer& renderer) {
	renderer.GetCamera().SetRect(FloatRect(0, 0, 16, 9));
	renderer.GetCamera().StopSmoothTransition();
	renderer.GetCamera().Update();
	FloatRect r = renderer.GetCamera().GetView().GetRect();

	sf::RectangleShape object;

	object.setTexture(&renderer.GetOutroTextures()[Renderer::OutroTextures::Background]);
	object.setPosition(ToSFMLVec2(r.xy + r.wh / 2.0f));
	object.setSize(ToSFMLVec2(r.wh));
	object.setOrigin(ToSFMLVec2(r.size / 2.0f));

	renderer.GetRenderTarget().draw(object);

	object.setTexture(&renderer.GetOutroTextures()[Renderer::OutroTextures::Building]);

	if (m_AnimationClock.getElapsedTime().asMilliseconds() > 5000) {
		float time = m_AnimationClock.getElapsedTime().asMilliseconds();
		float rotation = 180.0f * (time - 5000.0f) / 350.0f;
		if (rotation > 180.0f) rotation = 180.0f;
		object.setRotation(rotation);
	}

	if (m_AnimationClock.getElapsedTime().asMilliseconds() > 5350) {
		float time = m_AnimationClock.getElapsedTime().asMilliseconds();
		vec2 move = vec2(0, 1.0f * (time - 5350.0f) / 250.0f);
		object.setPosition(ToSFMLVec2(r.xy + r.wh / 2.0f + move));
	}

	if (m_AnimationClock.getElapsedTime().asMilliseconds() > 8000 && m_AnimationClock.getElapsedTime().asMilliseconds() < 9000 && !m_Fading) {
		renderer.FadeOut(sf::milliseconds(1000));
		m_Fading = true;
	}

	if (m_AnimationClock.getElapsedTime().asMilliseconds() > 9000) {
		if (m_Fading) {
			renderer.FadeIn(sf::milliseconds(500));
			m_Fading = false;
		}
		object.setTexture(&renderer.GetOutroTextures()[Renderer::OutroTextures::EndScreen]); 
		object.setPosition(ToSFMLVec2(r.xy + r.wh / 2.0f));
		object.setRotation(0);
	}

	renderer.GetRenderTarget().draw(object);

	if (m_AnimationClock.getElapsedTime().asSeconds() > 2 && !m_Explosion) {
		m_Explosion = true;

		std::shared_ptr<Explosion> explosion = std::make_shared<Explosion>();
		explosion->SetSize(vec2(r.size.y / 2.0f) * 3);

		g_ParticleSystem.Add(explosion, renderer.GetCamera().GetView().GetCenter(), true);
	}
}