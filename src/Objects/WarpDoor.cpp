#include "WarpDoor.hpp"
#include "../Player.hpp"

void WarpDoor::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::Player && self.tag == Collider::Tags::Interact) {
		m_Player = (Player*)other.parent;
		if (m_Player->IsInteracting()) {
			m_Entered = true;
			m_SpriteSwitchTimer.restart();
			m_Player->CancelJump();
		}
	}
}

void WarpDoor::Render(Renderer& renderer) {
	if (m_Entered && m_Player) {
		if (renderer.GetFadeStatus() == Renderer::Fade::InComplete) {
			renderer.FadeOut(sf::milliseconds(500));
			textureRect.x = textureRect.w;
		}
		else if (renderer.GetFadeStatus() == Renderer::Fade::OutComplete) {
			textureRect.x = 0;
			vec2 target;
			for (Collider& col : colliders) {
				FloatRect box = GetBoxColliderRect(col);
				if (col.tag == Collider::Tags::Default && col.isTrigger) {
					target = vec2(box.x + box.w / 2.0f - m_Player->transform.size.w / 2.0f, box.y + box.h - m_Player->transform.size.h);
				}
			}
			m_Player->Warp(target);
			renderer.FadeIn(sf::milliseconds(500));
			m_Entered = false;
		}
	}

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