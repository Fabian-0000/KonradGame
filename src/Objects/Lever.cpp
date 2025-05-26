#include "Lever.hpp"
#include "../Player.hpp"

bool Lever::IsActivated() {
	return m_Activated;
}

void Lever::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::Player && self.tag == Collider::Tags::Interact) {
		Player& player = *(Player*)other.parent;
		if (player.IsInteracting() && !m_Activated) {
			m_Activated = true;

			textureRect.x = m_Activated ? textureRect.w : 0;

			for (Collider& col : colliders) {
				if (col.tag == Collider::Tags::PowerOff || col.tag == Collider::Tags::PowerOn) {
					if (col.tag == Collider::Tags::PowerOff) col.tag = Collider::Tags::PowerOn;
					else if (col.tag == Collider::Tags::PowerOn) col.tag = Collider::Tags::PowerOff;
				}
			}
		}
	}
}

void Lever::Render(Renderer& renderer) {
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