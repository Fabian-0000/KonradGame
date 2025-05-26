#pragma once

#include <SFML/Graphics.hpp>
#include "Vector2.hpp"
#include <cmath>

template <class T>
class Rect {
public:
	union {
		struct {
			Vector2<T> position, size;
		};
		struct {
			Vector2<T> xy, wh;
		};
		struct {
			T x, y, w, h;
		};
		struct {
			T x1, y1, x2, y2;
		};
		struct {
			T r, g, b, a;
		};
		struct {
			T left, top, width, height;
		};
	};

	Rect() {
		x = 0;
		y = 0;
		w = 0; 
		h = 0;
	}

	Rect(T e) {
		x = e; 
		y = e;
		w = e;
		h = e;
	}

	Rect(Vector2<T> position, Vector2<T> size) {
		x = position.x;
		y = position.y;
		w = size.w;
		h = size.h;
	}

	Rect(T x, T y, T w, T h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	Rect(const Rect<T>& rect) {
		x = rect.x;
		y = rect.y;
		w = rect.w;
		h = rect.h;
	}

	Rect(const sf::Rect<T>& rect) {
		x = rect.left;
		y = rect.top;
		w = rect.width;
		h = rect.height;
	}

	template <typename U>
	Rect(const Rect<U>& rect) {
		x = (T)rect.x;
		y = (T)rect.y;
		w = (T)rect.w;
		h = (T)rect.h;
	}

	template <typename U>
	Rect(const sf::Rect<U>& rect) {
		x = (T)rect.left;
		y = (T)rect.top;
		w = (T)rect.width;
		h = (T)rect.height;
	}

	bool collides(const Rect<T>& rect) {
		return (x <= rect.x + rect.w && x + w >= rect.x && y <= rect.y + rect.h && y + h >= rect.y);
	}

	bool intersects(const Rect<T>& rect) {
		return (x < rect.x + rect.w && x + w > rect.x && y < rect.y + rect.h && y + h > rect.y);
	}

	Rect<T> GetOverlap(const Rect<T>& rect) {
		Rect<T> result;

		if (x > rect.x) result.x = x; else result.x = rect.x;
		if (y > rect.y) result.y = y; else result.y = rect.y;

		T r1 = x + w, r2 = rect.x + rect.w, b1 = y + h, b2 = rect.y + rect.h;

		result.w = std::max((T)0, std::min(r1, r2) - std::max(x, rect.x));
		result.h = std::max((T)0, std::min(b1, b2) - std::max(y, rect.y));

		return result;
	}

	bool collides(const Vector2<T>& v) {
		return (x <= v.x && x + w >= v.x && y <= v.y && y + h >= v.y);
	}

	bool intersects(const Vector2<T>& v) {
		return (x < v.x && x + w > v.x && y < v.y && y + h > v.y);
	}

	Vector2<T> GetCenter() {
		return Vector2<T>(x + w / 2.0f, y * h / 2.0f);
	}

	bool operator==(const Rect<T>& rect) {
		return x == rect.x && y == rect.y && w == rect.w && h == rect.h;
	}

	bool operator!=(const Rect<T>& rect) {
		return x != rect.x || y != rect.y || w != rect.w || h != rect.h;
	}
};

typedef Rect<uint64_t> UIntRect;
typedef Rect<int64_t> IntRect;
typedef Rect<float> FloatRect;
typedef Rect<float> Color;