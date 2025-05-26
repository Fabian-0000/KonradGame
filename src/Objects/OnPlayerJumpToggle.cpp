#include "OnPlayerJumpToggle.hpp"
#include "../Player.hpp"

void OnPlayerJumpToggle::Update(Input& input) {
	if (m_Player) {
		if (m_Player->IsStartingJump()) {
			Toggle();
		}
	}

	m_Player = nullptr;
}

void OnPlayerJumpToggle::Render(Renderer& renderer) {
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

void OnPlayerJumpToggle::OnCollision(const Collider& self, const Collider& other) {
	if (self.isTrigger && other.tag == Collider::Tags::Player) {
		m_Player = (Player*)other.parent;
	}
}

void OnPlayerJumpToggle::Toggle() {
	if (m_Exists) {
		for (Collider& col : colliders) {
			if (!col.isTrigger) col.enabled = false;
		}
		m_Exists = false;
	}
	else {
		for (Collider& col : colliders) {
			if (!col.isTrigger) col.enabled = true;
		}
		m_Exists = true;
	}
}