#include "Fan.hpp"
#include "../Player.hpp"
#include "../Time.hpp"

bool Fan::IsActivated() {
	return m_Activated;
}

void Fan::Update(Input& input) {
	if (!m_Activated) { 
		return; 
	}

	m_Animation.Update();

	textureRect.x = textureRect.w * m_Animation.GetIndex();
	if (m_Player) {
		if (m_Speed.x > 0.0f && m_Player->speed.x < 0.0f) {
			m_Player->speed.x = 0;
		}
		else m_Player->floatingSpeed += m_Speed;
	}

	m_Speed = 0;

	m_Time += Time.DeltaTime();
}

void Fan::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::Player) {
		m_Player = (Player*)other.parent;

		switch (self.tag) {
		case Collider::Tags::AirStreamNorth:
			m_Speed.y = -0.1; break;
		case Collider::Tags::AirStreamSouth:
			m_Speed.y = 0.1; break;
		case Collider::Tags::AirStreamEast:
			m_Speed.x = 0.1; break;
		case Collider::Tags::AirStreamWest:
			m_Speed.x = -0.1; break;
		}
	}

	if (other.tag == Collider::Tags::PowerOff) {
		m_Activated = false;
	}
	if (other.tag == Collider::Tags::PowerOn) {
		m_Activated = true;
	}
}

void Fan::Render(Renderer& renderer) {
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

	renderer.GetRenderTarget().draw(object);

	if (!m_Activated) return;

	for (Collider& col : colliders) {
		if (col.tag == Collider::Tags::AirStreamEast ||
			col.tag == Collider::Tags::AirStreamWest) {

			sf::RectangleShape object2;

			//object2.setFillColor(sf::Color(0, 255, 0, 100));
			object2.setTexture(&renderer.GetParticleTextures()[Renderer::ParticleTextures::WindHorizontal]);
			FloatRect texrect;
			texrect.x = 16 * ((uint64_t)(m_Time / 0.15f) % 3);
			texrect.y = 0;
			texrect.w = 20 / GetBoxColliderRect(col).h * GetBoxColliderRect(col).w;
			texrect.h = 20;
			object2.setTextureRect(ToSFMLIntRect(texrect));
			object2.setPosition(ToSFMLVec2(GetBoxColliderRect(col).xy));
			object2.setSize(ToSFMLVec2(GetBoxColliderRect(col).wh));

			renderer.GetRenderTarget().draw(object2);
		}
		if (col.tag == Collider::Tags::AirStreamNorth ||
			col.tag == Collider::Tags::AirStreamSouth) {

			sf::RectangleShape object2;

			object2.setTexture(&renderer.GetParticleTextures()[Renderer::ParticleTextures::WindVertical]);
			FloatRect texrect;
			texrect.x = 20 * ((uint64_t)(m_Time / 0.15f) % 3);
			texrect.y = 0;
			texrect.w = 20;
			texrect.h = 20 / GetBoxColliderRect(col).w * GetBoxColliderRect(col).h;
			object2.setTextureRect(ToSFMLIntRect(texrect));
			object2.setPosition(ToSFMLVec2(GetBoxColliderRect(col).xy));
			object2.setSize(ToSFMLVec2(GetBoxColliderRect(col).wh));

			renderer.GetRenderTarget().draw(object2);
		}
	}
}