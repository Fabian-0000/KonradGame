#pragma once

#include "Vector2.hpp"
#include "Rect.hpp"

struct Transform {
	Transform() {}

	union {
		struct {
			vec2 position;
			vec2 size;
		};

		FloatRect rect;
	};
};