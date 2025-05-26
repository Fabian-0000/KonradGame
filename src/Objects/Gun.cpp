#include "Gun.hpp"
#include "GlassBox.hpp"
#include "../Player.hpp"
#include "../SaveData.hpp"

void Gun::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::Player) {
		Player& player = *(Player*)other.parent;
		if (player.IsInteracting() && !m_BoxClosed) {
			m_PickedUp = true;
			g_SaveData.hasShotgun = true;
		}
	}

	if (other.parent->id == Object::Id::GlassBox) {
		GlassBox& box = *(GlassBox*)other.parent;
		if (box.IsDestroyed()) {
			m_BoxClosed = false;
			for (Collider& col : colliders) {
				col.tag = Collider::Tags::Interact;
			}
		}
	}
}

void Gun::Render(Renderer& renderer) {
	if (m_PickedUp) return;

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