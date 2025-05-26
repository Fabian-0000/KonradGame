#include "Gravitational.hpp"
#include "../Time.hpp"
#include "../Particle.hpp"

Gravitational::Gravitational() {
	Collider groundCheck;
	groundCheck.type = ColliderType::BoxCollider;
	groundCheck.box = FloatRect(0.0001f, 0.9f, 0.9998f, 0.105f);
	groundCheck.isTrigger = true;
	groundCheck.tag = Collider::Tags::GroundCheck;
	groundCheck.parent = this;

	colliders.push_back(groundCheck);
}

void Gravitational::Update(Input& input) {
	type = ObjectType::HalfDynamic;

	const float gravityAcc = 0.005f;

	if (!m_Grounded) {
		m_YSpeed += gravityAcc * Time.DeltaTime() * 60.0f;
		if (m_YSpeed > 0.1f) {
			m_YSpeed = 0.1f;
		}
	}
	else {
		m_YSpeed = 0.0f;
	}

	transform.position.y += m_YSpeed * Time.DeltaTime() * 60.0f;

	m_Grounded = false;
}

void Gravitational::Render(Renderer& renderer) {
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

void Gravitational::OnCollision(const Collider& self, const Collider& other) {
	if (self.tag == Collider::Tags::GroundCheck && other.parent != this && !other.isTrigger) {
		m_Grounded = true;
	}
}