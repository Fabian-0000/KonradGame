#include "Chandelier.hpp"
#include "../Time.hpp"
#include "../Player.hpp"
#include "../Particle.hpp"

void Chandelier::Update(Input& input) {
	if (!m_Exists) return;

	if (m_Falling) {
		textureRect.x = textureRect.w;
	}
	else {
		textureRect.x = 0;
		return;
	}

	const float gravityAcc = 0.005f;

	m_YSpeed += gravityAcc * Time.DeltaTime() * 60.0f;
	if (m_YSpeed > 0.1f) {
		m_YSpeed = 0.1f;
	}

	transform.position.y += m_YSpeed * Time.DeltaTime() * 60.0f;
}

void Chandelier::Render(Renderer& renderer) {
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

void Chandelier::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::Player && self.tag == Collider::Tags::Dashable) {
		Player& player = *(Player*)other.parent;
		if (player.IsDashing()) {
			m_Falling = true;
		}
	}
	if (other.tag == Collider::Tags::Default) {
		if (other.parent->id == Object::Id::GlassBox && m_Exists) {
			m_Exists = false;
			vec2 target = transform.position;
			target.x += transform.size.x / 2.0f;
			target.y += transform.size.y;
			g_ParticleSystem.Add(std::make_shared<Explosion>(), target, true);
		}
	}
}