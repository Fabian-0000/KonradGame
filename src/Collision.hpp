#pragma once

#include "Vector2.hpp"

bool RectCollide(vec2 p1, vec2 s1, float p1_response, vec2* p1_movement, vec2* p1_rect_pos, vec2* p1_rect_size, vec2 p2, vec2 s2, float p2_response, vec2* p2_movement, vec2* p2_rect_pos, vec2* p2_rect_size);

namespace SAT {
	vec2 ConvertPosition(vec2 vector, vec2* object_pos, vec2* object_size);

	bool Collide(vec2* p1, size_t p1_size, float p1_response, vec2* p1_movement, vec2* p1_rect_pos, vec2* p1_rect_size, vec2* p2, size_t p2_size, float p2_response, vec2* p2_movement, vec2* p2_rect_pos, vec2* p2_rect_size);

	bool CircleCollide(vec2* p1, size_t p1_size, float p1_response, vec2* p1_movement, vec2* p1_rect_pos, vec2* p1_rect_size, vec2 p2, float r2, float p2_response, vec2* p2_movement, vec2* p2_rect_pos, vec2* p2_rect_size);
}

bool CircCollide(vec2 p1, float r1, float p1_response, vec2* p1_movement, vec2* p1_rect_pos, vec2* p1_rect_size, vec2 p2, float r2, float p2_response, vec2* p2_movement, vec2* p2_rect_pos, vec2* p2_rect_size);