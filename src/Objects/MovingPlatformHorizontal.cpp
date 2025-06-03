#include "MovingPlatformHorizontal.hpp"

#include "../Player.hpp"

#include "../Time.hpp"

#include "MovingPlatform.hpp"

#include <cmath>

void MovingPlatformHorizontal::Update(Input& input) {
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

		m_LaneSize = rect.w;

		m_Amplitude = std::abs(rect.w) / 2.0f - size.w / 2.0f;
		m_Center = rect.x + rect.w / 2.0f - transform.size.w / 2.0f;
		m_Started = true;
		m_StartAngle = std::asin(std::clamp((size.x - m_Center) / m_Amplitude, -1.0f, 1.0f));
	}

	if (!g_Activated) { 
		m_PlayerTouched = false; 
		m_Clock = 0.0f;
	}

	if (m_PlayerTouched) m_Clock += Time.DeltaTime();

	float pos = m_Center + m_Amplitude * std::sin(2.f * 3.141592f * (1.0f / m_LaneSize) * m_Clock + m_StartAngle);

	if (m_Player) {
		m_Player->transform.position.x += pos - transform.position.x;
		m_Player->lastPosition.x += pos - transform.position.x;
	}

	transform.position.x = pos;

	m_Player = nullptr;
}

void MovingPlatformHorizontal::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::PlayerGround && !self.isTrigger) {
		m_Player = (Player*)other.parent;
		m_PlayerTouched = true;
		g_Activated = true;
	}

	if ((other.tag == Collider::Tags::Player || other.tag == Collider::Tags::PlayerTop) && !self.isTrigger) {
		m_PlayerTouched = true;
		g_Activated = true;
	}
}