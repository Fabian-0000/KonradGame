#include "View.hpp"

sf::View& View::GetSFMLView() {
	return m_SFMLView;
}

vec2 View::MapCoordsToPixelIntern(sf::RenderTarget& rt, vec2 coord) const {
	sf::View view = rt.getView();

	// First, transform the point by the view matrix
	vec2 normalized = view.getTransform().transformPoint(sf::Vector2f(coord.x, coord.y));

	// Then convert to viewport coordinates
	vec2 pixel;
	sf::FloatRect viewport = sf::FloatRect(rt.getViewport(view));
	pixel.x = (normalized.x + 1.f) / 2.f * viewport.width + viewport.left;
	pixel.y = (-normalized.y + 1.f) / 2.f * viewport.height + viewport.top;

	return pixel;
}

vec2 View::MapPixelToCoordsIntern(sf::RenderTarget& rt, vec2 point) const {
	sf::View view = rt.getView();

	// First, convert from viewport coordinates to homogeneous coordinates
	sf::Vector2f normalized;
	sf::FloatRect viewport = sf::FloatRect(rt.getViewport(view));
	normalized.x = -1.f + 2.f * (point.x - viewport.left) / viewport.width;
	normalized.y = 1.f - 2.f * (point.y - viewport.top) / viewport.height;

	// Then transform by the inverse of the view matrix
	return view.getInverseTransform().transformPoint(normalized);
}

vec2 View::ConvertPixelToCoords(sf::RenderTarget& target, vec2 input) {
	target.setView(m_SFMLView);

	return MapPixelToCoordsIntern(target, input);
}

FloatRect View::ConvertPixelToCoords(sf::RenderTarget& target, FloatRect input) {
	target.setView(m_SFMLView);

	vec2 pos = MapPixelToCoordsIntern(target, input.xy);
	vec2 size = MapPixelToCoordsIntern(target, vec2(input.x + input.w, input.y + input.h));

	size -= pos;

	return FloatRect(pos, size);
}

vec2 View::ConvertCoordsToPixel(sf::RenderTarget& target, vec2 input) {
	target.setView(m_SFMLView);

	return MapCoordsToPixelIntern(target, input);
}

FloatRect View::ConvertCoordsToPixel(sf::RenderTarget& target, FloatRect input) {
	target.setView(m_SFMLView);

	vec2 pos = MapCoordsToPixelIntern(target, input.xy);
	vec2 size = MapCoordsToPixelIntern(target, vec2(input.x + input.w, input.y + input.h));

	size -= pos;

	return FloatRect(pos, size);
}

View View::CreateViewFromRect(sf::RenderTarget& target, FloatRect rect, bool limited, float innerWidth) {
	View result;

	float oldWidth = rect.w;

	FloatRect vp = ConvertCoordsToPixel(target, rect);

	vp.xy /= target.getSize();
	vp.wh /= target.getSize();

	if (innerWidth > 0.0f) {
		float innerHeight = innerWidth * rect.h / rect.w;
		rect.wh = vec2(innerWidth, innerHeight);
	}

	if (!limited) {
		result.m_SFMLView.setViewport(sf::FloatRect(vp.x, vp.y, vp.w, vp.h));
		result.m_SFMLView.setSize(rect.w, rect.h);
		result.m_SFMLView.setCenter(rect.w / 2.0f, rect.h / 2.0f);
	}
	else {
		FloatRect viewport = m_SFMLView.getViewport();

		FloatRect vp2 = vp.GetOverlap(viewport);

		vec2 size = rect.wh * vp2.wh / vp.wh;

		vec2 pos = rect.xy;
		pos -= ConvertPixelToCoords(target, viewport.xy * target.getSize());

		if (innerWidth > 0.0f) pos *= innerWidth / oldWidth;

		if (pos.x > 0) pos.x = 0; else pos.x = -pos.x;
		if (pos.y > 0) pos.y = 0; else pos.y = -pos.y;

		result.m_SFMLView.setViewport(sf::FloatRect(vp2.x, vp2.y, vp2.w, vp2.h));
		result.m_SFMLView.setSize(size.x, size.y);
		result.m_SFMLView.setCenter(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f);
	}

	return result;
}

void View::SetRect(FloatRect rect) {
	SetSize(rect.wh);
	SetPosition(rect.xy);
}

FloatRect View::GetRect() {
	return FloatRect(GetPosition(), GetSize());
}

void View::SetSize(vec2 size) {
	m_SFMLView.setSize(sf::Vector2f(size.x, size.y));
}

void View::SetSize(sf::RenderTarget& target, vec2 pixel, vec2 size) {
	vec2 beforeCoord = MapPixelToCoordsIntern(target, pixel);
	sf::View view{ m_SFMLView };
	view.setSize(sf::Vector2f(size.x, size.y));
	target.setView(view);
	vec2 afterCoord = MapPixelToCoordsIntern(target, pixel);
	const vec2 offsetCoords = beforeCoord - afterCoord;
	view.move(sf::Vector2f(offsetCoords.x, offsetCoords.y));
	m_SFMLView = view;
}

vec2 View::GetSize() {
	return m_SFMLView.getSize();
}

void View::SetCenter(vec2 center) {
	m_SFMLView.setCenter(sf::Vector2f(center.x, center.y));
}

vec2 View::GetCenter() {
	return m_SFMLView.getCenter();
}

void View::SetPosition(vec2 pos) {
	pos += GetSize() / 2.0f;
	m_SFMLView.setCenter(sf::Vector2f(pos.x, pos.y));
}

vec2 View::GetPosition() {
	return vec2(m_SFMLView.getCenter()) - GetSize() / 2.0f;
}