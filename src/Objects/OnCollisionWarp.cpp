#include "OnCollisionWarp.hpp"

#include "../Player.hpp"

void OnCollisionWarp::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::Player && self.tag == Collider::Tags::Warp) {
		m_Player = (Player*)other.parent;
		m_Entered = true;
		m_Player->CancelJump();
	}
}

void OnCollisionWarp::Render(Renderer& renderer) {
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
}