#include "Renderer.hpp"

Renderer::Renderer(sf::RenderTarget& rt) : m_RenderTarget(rt) {
	m_OverlayView.setSize(192.0f, 108.0f);
	m_OverlayView.setCenter(192.0f / 2.0f, 108.0f / 2.0f);
}

sf::RenderTarget& Renderer::GetRenderTarget() {
	m_RenderTarget.setView(m_Camera.GetView().GetSFMLView());
	return m_RenderTarget;
}

sf::RenderTarget& Renderer::GetOverlayRenderTarget() {
	m_RenderTarget.setView(m_OverlayView);
	return m_RenderTarget;
}

std::vector<sf::Texture>& Renderer::GetTextures() {
	return m_Textures;
}

std::vector<sf::Texture>& Renderer::GetPlayerTextures() {
	return m_PlayerTextures;
}

std::vector<sf::Texture>& Renderer::GetEnemyTextures() {
	return m_EnemyTextures;
}

std::vector<sf::Texture>& Renderer::GetElevatorTextures() {
	return m_ElevatorTextures;
}

std::vector<sf::Texture>& Renderer::GetParticleTextures() {
	return m_ParticleTextures;
}

std::vector<sf::Texture>& Renderer::GetOutroTextures() {
	return m_OutroTextures;
}

std::vector<sf::Texture>& Renderer::GetMenuTextures() {
	return m_MenuTextures;
}

View& Renderer::GetView() {
	return m_Camera.GetView();
}

Camera& Renderer::GetCamera() {
	return m_Camera;
}

void Renderer::FadeOut(sf::Time time) {
	m_FadeStatus = Fade::Out;
	m_FadeTimer.restart();
	m_FadeTime = time;
}

void Renderer::FadeIn(sf::Time time) {
	m_FadeStatus = Fade::In;
	m_FadeTimer.restart();
	m_FadeTime = time;
}

void Renderer::RenderFade() {
	float alpha = 0.0f;
	if (m_FadeStatus == Fade::In) {
		alpha = 255.0f - (m_FadeTimer.getElapsedTime() / m_FadeTime) * 255.0f;

		if (m_FadeTimer.getElapsedTime() >= m_FadeTime) {
			m_FadeStatus = Fade::InComplete;
		}
	}
	if (m_FadeStatus == Fade::Out) {
		alpha = (m_FadeTimer.getElapsedTime() / m_FadeTime) * 255.0f;

		if (m_FadeTimer.getElapsedTime() >= m_FadeTime) {
			alpha = 255.0f;
			m_FadeStatus = Fade::OutComplete;
		}
	}

	if (m_FadeStatus == Fade::InComplete) {
		alpha = 0.0f;
	}
	if (m_FadeStatus == Fade::OutComplete) {
		alpha = 255.0f;
	}

	sf::RectangleShape rs;
	rs.setPosition(ToSFMLVec2(GetView().GetPosition()));
	rs.setSize(ToSFMLVec2(GetView().GetSize()));
	rs.setFillColor(sf::Color(0, 0, 0, alpha));

	GetRenderTarget().draw(rs);
}

Renderer::Fade Renderer::GetFadeStatus() {
	return m_FadeStatus;
}