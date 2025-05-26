#include "OnTimerToggle.hpp"
#include "../Time.hpp"

void OnTimerToggle::Toggle() {
	if (m_Exists) {
		for (Collider& col : colliders) {
			col.enabled = false;
		}
		m_Exists = false;
	}
	else {
		for (Collider& col : colliders) {
			col.enabled = true;
		}
		m_Exists = true;
	}
}

void OnTimerToggle::Update(Input& input) {
	m_Clock += Time.DeltaTime();

	if (m_Clock > 2.0f) {
		m_Clock -= 2.0f;
		Toggle();
	}
}

void OnTimerToggle::Render(Renderer& renderer) {
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