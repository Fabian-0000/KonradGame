#include "DisappearingPlatform.hpp"

void DisappearingPlatform::Update(Input& input) {
	if (m_Touched && m_Timer.getElapsedTime().asSeconds() > 0.5f) {
		m_Exists = false;

		for (Collider& col : colliders) {
			col.enabled = false;
		}
	}

	if (!m_Exists && m_Timer.getElapsedTime().asSeconds() > 5.0f) {
		for (Collider& col : colliders) {
			col.enabled = true;
		}

		m_Touched = false;
		m_Exists = true;
	}
}

void DisappearingPlatform::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::PlayerGround && !m_Touched) {
		m_Timer.restart();
		m_Touched = true;
	}
}

void DisappearingPlatform::Render(Renderer& renderer) {
	if (!m_Exists) return;
	if (!hasTexture && color.a == 0.0f) return;

	sf::RectangleShape object;

	if (hasTexture) {
		object.setTexture(&renderer.GetTextures()[textureIndex]);
		object.setTextureRect(ToSFMLIntRect(textureRect));
	}
	else {
		object.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
	}
	object.setPosition(ToSFMLVec2(transform.position));
	object.setSize(ToSFMLVec2(transform.size));

	renderer.GetRenderTarget().setView(renderer.GetView().GetSFMLView());
	renderer.GetRenderTarget().draw(object);
}