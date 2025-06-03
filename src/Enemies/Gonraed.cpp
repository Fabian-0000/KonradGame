#include "Gonraed.hpp"

#include "../Time.hpp"
#include "../Particle.hpp"

#define GRAVITY_ACC 0.015f

void Gonraed::Update(Input& input) {
	m_AnimationIdle.Update();
	m_AnimationPunch.Update();
	m_AnimationWalk.Update();

	if (m_Player) {
		m_Walking = true;

		if (m_TurnClock.getElapsedTime().asSeconds() > 1.0f) {
			if (m_Player->transform.position.x < transform.position.x) {
				m_Flip = true;
				m_Speed.x = -1.5f;
			}
			else {
				m_Flip = false;
				m_Speed.x = 1.5f;
			}
			m_TurnClock.restart();
		}
	}
	else {
		m_Speed.x = 0.0f;
		m_Walking = false;
	}

	if (m_Punching && m_AnimationPunch.IsOver()) {
		m_Punching = false;
	}

	const float gravityAcc = GRAVITY_ACC;

	m_Speed.y += gravityAcc * Time.DeltaTime() * 60.0f;

	if (m_Grounded) {
		m_Speed.y = 0.00f;
	}

	if (m_Speed.y > 0.2f) {
		m_Speed.y = 0.2f;
	}

	transform.position.x += m_Speed.x * Time.DeltaTime();
	transform.position.y += m_Speed.y * Time.DeltaTime() * 60.0f;

	m_Player = nullptr;
	m_Grounded = false;
}

void Gonraed::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::Player && (self.tag == Collider::Tags::HighRange || self.tag == Collider::Tags::LowRange)) {
		m_Player = (Player*)other.parent;

		if (self.tag == Collider::Tags::LowRange) {
			if (m_Punching && m_AnimationPunch.GetIndex() == 2) {
				((Player*)other.parent)->Damage();
			}
		}

		if (self.tag == Collider::Tags::LowRange && !m_Punching && m_PunchCooldown.getElapsedTime().asMilliseconds() > 1000) {
			m_Punching = true;
			m_AnimationPunch.Reset();
			m_PunchCooldown.restart();
		}
	}

	if (other.tag == Collider::Tags::PlayerShot && self.tag == Collider::Tags::Damage) {
		exists = false;
		g_ParticleSystem.Add(std::make_shared<Explosion>(), transform.position + transform.size / 2.0f, true);
		return;
	}

	if (self.tag == Collider::Tags::GroundCheck && !other.isTrigger) {
		m_Grounded = true;
	}

	if (other.tag == Collider::Tags::Death) {
		exists = false;
		return;
	}
}

void Gonraed::Render(Renderer& renderer) {
	transform.size = vec2(2.833333f, 2.666666f);

	textureRect.w = 34;
	textureRect.h = 32;

	Animation* currentAnimation = &m_AnimationOneFrame;

	sf::RectangleShape object;

	if (m_Walking) {
		object.setTexture(&renderer.GetEnemyTextures()[Renderer::EnemyTextures::GonraedWalkLegs]);
		currentAnimation = &m_AnimationWalk;
	}
	else {
		object.setTexture(&renderer.GetEnemyTextures()[Renderer::EnemyTextures::GonraedIdleLegs]);
		currentAnimation = &m_AnimationOneFrame;
	}
	textureRect.x = textureRect.w * currentAnimation->GetIndex();
	object.setTextureRect(ToSFMLIntRect(textureRect));
	object.setFillColor(sf::Color::White);
	object.setScale(sf::Vector2f(m_Flip ? -1 : 1, 1));
	object.setOrigin(ToSFMLVec2(transform.size) / 2.0f);

	object.setPosition(ToSFMLVec2(transform.position) + ToSFMLVec2(transform.size) / 2.0f);
	object.setSize(ToSFMLVec2(transform.size));

	renderer.GetRenderTarget().draw(object);

	if (m_Punching) {
		if (currentAnimation == &m_AnimationWalk) {
			uint8_t index = m_AnimationWalk.GetIndex();
			if (index == 1 || index == 4) {
				textureRect.y = -1;
			}
		}
	}

	if (m_Punching) {
		object.setTexture(&renderer.GetEnemyTextures()[Renderer::EnemyTextures::GonraedPunchBody]);
		currentAnimation = &m_AnimationPunch;
	}
	else if (m_Walking) {
		object.setTexture(&renderer.GetEnemyTextures()[Renderer::EnemyTextures::GonraedWalkBody]);
		currentAnimation = &m_AnimationWalk;
	}
	else {
		object.setTexture(&renderer.GetEnemyTextures()[Renderer::EnemyTextures::GonraedIdleBody]);
		currentAnimation = &m_AnimationIdle;
	}

	textureRect.x = textureRect.w * currentAnimation->GetIndex();

	object.setTextureRect(ToSFMLIntRect(textureRect));

	renderer.GetRenderTarget().draw(object);

	textureRect.y = 0;

	object.setFillColor(sf::Color::White);

	renderer.GetRenderTarget().draw(object);

	/**********************************************************************/

	/*sf::RectangleShape object2;

	object2.setFillColor(sf::Color(0, 0, 255, 50));
	object2.setPosition(ToSFMLVec2(GetBoxColliderRect(colliders[0]).xy));
	object2.setSize(ToSFMLVec2(GetBoxColliderRect(colliders[0]).wh));

	renderer.GetRenderTarget().setView(renderer.GetView().GetSFMLView());
	renderer.GetRenderTarget().draw(object2);

	object2.setFillColor(sf::Color(255, 0, 0, 50));
	object2.setPosition(ToSFMLVec2(GetBoxColliderRect(colliders[1]).xy));
	object2.setSize(ToSFMLVec2(GetBoxColliderRect(colliders[1]).wh));

	renderer.GetRenderTarget().setView(renderer.GetView().GetSFMLView());
	renderer.GetRenderTarget().draw(object2);

	object2.setFillColor(sf::Color(0, 255, 0, 50));
	object2.setPosition(ToSFMLVec2(GetBoxColliderRect(colliders[2]).xy));
	object2.setSize(ToSFMLVec2(GetBoxColliderRect(colliders[2]).wh));

	renderer.GetRenderTarget().setView(renderer.GetView().GetSFMLView());
	renderer.GetRenderTarget().draw(object2);*/
}