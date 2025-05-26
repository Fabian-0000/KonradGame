#include "OnPowerDisappear.hpp"

void OnPowerDisappear::Update(Input& input) {
	if (!m_Powered) {
		for (size_t i = 0; i < colliders.size(); i++) {
			if (!colliders[i].isTrigger)
				colliders[i].enabled = true;
		}
	}
	else {
		for (size_t i = 0; i < colliders.size(); i++) {
			if (!colliders[i].isTrigger)
				colliders[i].enabled = false;
		}
	}

	m_Powered = false;
}

void OnPowerDisappear::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::PowerOn) {
		m_Powered = true;
	}
}

void OnPowerDisappear::Render(Renderer& renderer) {
	if (m_Powered) return;

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