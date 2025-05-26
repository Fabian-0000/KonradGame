#pragma once

#include <SFML/Graphics.hpp>
#include "Vector2.hpp"
#include "Rect.hpp"
#include "Mouse.hpp"

class View {
public:
	vec2 ConvertPixelToCoords(sf::RenderTarget& target, vec2 input);

	FloatRect ConvertPixelToCoords(sf::RenderTarget& target, FloatRect input);

	vec2 ConvertCoordsToPixel(sf::RenderTarget& target, vec2 input);

	FloatRect ConvertCoordsToPixel(sf::RenderTarget& target, FloatRect input);

	View CreateViewFromRect(sf::RenderTarget& target, FloatRect rect, bool limited = true, float innerWidth = 0.0f);

	sf::View& GetSFMLView();

	void SetRect(FloatRect rect);

	FloatRect GetRect();

	void SetSize(vec2 size);

	void SetSize(sf::RenderTarget& target, vec2 pixel, vec2 size);

	vec2 GetSize();

	void SetCenter(vec2 center);

	vec2 GetCenter();

	void SetPosition(vec2 pos);

	vec2 GetPosition();

protected:
	sf::View m_SFMLView;

private:
	vec2 MapCoordsToPixelIntern(sf::RenderTarget& rt, vec2 coord) const;

	vec2 MapPixelToCoordsIntern(sf::RenderTarget& rt, vec2 coord) const;
};