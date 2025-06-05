#pragma once

#include "Vector2.hpp"
#include "Rect.hpp"

template <class T>
struct TransformT {
	TransformT() {}

	union {
		struct {
			Vector2<T> position;
			Vector2<T> size;
		};

		Rect<T> rect;
	};
};

typedef TransformT<float> Transform;