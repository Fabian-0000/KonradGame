#include "MovingPlatformVertical.hpp"

#include "../Player.hpp"

#include "../Time.hpp"

#include "MovingPlatform.hpp"

void MovingPlatformVertical::Update(Input& input) {
	if (!m_Started) {
		FloatRect rect;
		FloatRect size;

		for (Collider& col : colliders) {
			if (col.tag == Collider::Tags::Lane) {
				rect = col.parent->GetBoxColliderRect(col);
			}
			if (col.tag == Collider::Tags::Default) {
				size = col.parent->GetBoxColliderRect(col);
			}
		}

		m_LaneSize = rect.h;

		m_Amplitude = std::abs(rect.h) / 2.0f - size.h / 2.0f;
		m_Center = rect.y + rect.h / 2.0f - transform.size.h / 2.0f;
		m_Started = true;
		m_StartAngle = std::asinf(std::clamp((size.y - m_Center) / m_Amplitude, -1.0f, 1.0f));
	}

	m_Clock += Time.DeltaTime();

	float pos = m_Center + m_Amplitude * std::sin(2.f * 3.141592f * (1.0f / m_LaneSize) * m_Clock + m_StartAngle);

	if (m_Player) {
		m_Player->transform.position.y += pos - transform.position.y;
	}

	transform.position.y = pos;

	m_Player = nullptr;
}

void MovingPlatformVertical::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::PlayerGround && !self.isTrigger) {
		m_Player = (Player*)other.parent;
	}
}