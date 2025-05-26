#include "Camera.hpp"
#include "Time.hpp"

void Camera::Shake(sf::Time time) {
	m_ShakingClock.restart();
	m_ShakingTime = time;
	m_Shaking = true;
}

void Camera::CancelShake() {
	m_Shaking = false;
}

void Camera::StayInRect(bool value) {
	m_StayInRect = value;
}

void Camera::SetRect(FloatRect rect) {
	m_Rect = rect;
}

void Camera::SetStartRect(FloatRect rect) {
	m_Rect = rect;
	m_OldRect = rect;
}

void Camera::SetSize(vec2 size) {
	m_Size = size;
}

void Camera::Target(vec2 pos) {
	m_Target = pos;
}

MainView& Camera::GetView() {
	return m_View;
}

void Camera::DoSmoothTransition() {
	m_SmoothTransitionCalled = true;
	m_SmoothTransition = true;
	m_OldRect = m_View.GetRect();
	m_StateChangedClock.restart();
}

void Camera::StopSmoothTransition() {
	m_SmoothTransitionCalled = false;
	m_SmoothTransition = false;
}

void Camera::Update() {
	m_View.SetCenter(m_Target);
	m_View.SetSize(m_Size);
	FloatRect rect = m_View.GetRect();

	if (m_StayInRect) {
		if (rect.w > m_Rect.w) rect.w = m_Rect.w;
		if (rect.h > m_Rect.h) rect.h = m_Rect.h;

		if (rect.x < m_Rect.x) rect.x = m_Rect.x;
		if (rect.y < m_Rect.y) rect.y = m_Rect.y;

		if (rect.x + rect.w > m_Rect.x + m_Rect.w) rect.x = m_Rect.x + m_Rect.w - rect.w;
		if (rect.y + rect.h > m_Rect.y + m_Rect.h) rect.y = m_Rect.y + m_Rect.h - rect.h;

		if (rect.w < 16.0f) rect.w = 16.0f;
		if (rect.h < 9.0f) rect.h = 9.0f;
	}

	float fac = 0.01f * m_StateChangedClock.getElapsedTime().asSeconds() * 20 * Time.DeltaTime() * 60.0f;
	if (std::abs((rect.xy - m_OldRect.xy).y) <= 0.01f && m_SmoothTransition) {
		m_View.SetRect(rect);
		StopSmoothTransition();
	}
	if (fac > 1.0f || !m_SmoothTransitionCalled) fac = 1.0f;

	m_OldRect.x = m_OldRect.x + (rect.x - m_OldRect.x) * fac;
	m_OldRect.y = m_OldRect.y + (rect.y - m_OldRect.y) * fac;
	rect = FloatRect(m_OldRect.x, m_OldRect.y, rect.w, rect.h);

	if (m_Shaking && m_ShakingClock.getElapsedTime() < m_ShakingTime) {
		rect.x += ((std::rand() % 100) / 400.0f - 0.125f) / 32.0f;
		rect.y += ((std::rand() % 100) / 400.0f - 0.125f) / 32.0f;
	}

	m_View.SetRect(rect);
	
	m_SmoothTransition = false;
}