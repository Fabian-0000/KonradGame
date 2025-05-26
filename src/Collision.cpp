#include "Vector2.hpp"
#include "Collision.hpp"
#include <algorithm>

bool RectCollide(vec2 p1, vec2 s1, float p1_response, vec2* p1_movement, vec2* p1_rect_pos, vec2* p1_rect_size, vec2 p2, vec2 s2, float p2_response, vec2* p2_movement, vec2* p2_rect_pos, vec2* p2_rect_size) {	
	vec2 defp1 = p1, defp2 = p2;
	
	if (p1_rect_pos && p1_rect_size) {
		p1 = SAT::ConvertPosition(p1, p1_rect_pos, p1_rect_size);
		s1 = SAT::ConvertPosition(defp1+s1, p1_rect_pos, p1_rect_size) - p1;
	}

	if (p2_rect_pos && p2_rect_size) {
		p2 = SAT::ConvertPosition(p2, p2_rect_pos, p2_rect_size);
		s2 = SAT::ConvertPosition(defp2 + s2, p2_rect_pos, p2_rect_size) - p2;
	}
	
	bool result = (p1.x < p2.x + s2.x && p1.x + s1.x > p2.x && p1.y < p2.y + s2.y && p1.y + s1.y > p2.y);

	if (!result) return false;

	vec2 overlap(std::min(p2.x + s2.x - p1.x, p1.x + s1.x - p2.x), std::min(p2.y + s2.y - p1.y, p1.y + s1.y - p2.y));

	vec2 movement;

	if (overlap.y > overlap.x) {
		movement = vec2(overlap.x, 0.0f);
		if (p2.x > p1.x + s1.x / 2.0f) movement.x *= -1;
	}
	else {
		movement = vec2(0.0f, overlap.y);
		if (p2.y > p1.y + s1.y / 2.0f) movement.y *= -1;
	}

	if (p1_movement) {
		*p1_movement = movement * p1_response;
	}
	if (p2_movement) {
		*p2_movement = movement * -p2_response;
	}

	return result;
}

namespace SAT {
	vec2 ConvertPosition(vec2 vector, vec2* object_pos, vec2* object_size) {
		if (!object_pos || !object_size) return vector;

		return vec2(object_pos->x + object_size->x * vector.x, object_pos->y + object_size->y * vector.y);
	}

	bool Collide(vec2* p1, size_t p1_size, float p1_response, vec2* p1_movement, vec2* p1_rect_pos, vec2* p1_rect_size, vec2* p2, size_t p2_size, float p2_response, vec2* p2_movement, vec2* p2_rect_pos, vec2* p2_rect_size) {
		bool collision = true;

		float overlap = INFINITY;
		bool first = false;
		vec2 axis;

		float realOverlap = 0.0f;

		for (size_t i = 0; i < p1_size + p2_size; i++) {
			vec2 point1, point2;

			if (i < p1_size) {
				point1 = ConvertPosition(p1[i], p1_rect_pos, p1_rect_size);
				point2 = ConvertPosition(p1[(i + 1) % p1_size], p1_rect_pos, p1_rect_size);

			}
			else {
				point1 = ConvertPosition(p2[i - p1_size], p2_rect_pos, p2_rect_size);
				point2 = ConvertPosition(p2[(i + 1 - p1_size) % p2_size], p2_rect_pos, p2_rect_size);
			}

			vec2 proj_axis = vec2(-point2.y + point1.y, point2.x - point1.x);
			proj_axis = proj_axis / sqrt(proj_axis.x * proj_axis.x + proj_axis.y * proj_axis.y);

			float proj_p1_min = INFINITY, proj_p1_max = -INFINITY, proj_p2_min = INFINITY, proj_p2_max = -INFINITY;

			for (size_t j = 0; j < p1_size; j++) {
				vec2 p1c = ConvertPosition(p1[j], p1_rect_pos, p1_rect_size);
				float proj = p1c.x * proj_axis.x + p1c.y * proj_axis.y;

				proj_p1_min = std::min(proj_p1_min, proj);
				proj_p1_max = std::max(proj_p1_max, proj);
			}
			for (size_t j = 0; j < p2_size; j++) {
				vec2 p2c = ConvertPosition(p2[j], p2_rect_pos, p2_rect_size);
				float proj = p2c.x * proj_axis.x + p2c.y * proj_axis.y;

				proj_p2_min = std::min(proj_p2_min, proj);
				proj_p2_max = std::max(proj_p2_max, proj);
			}

			if ((proj_p1_min < proj_p2_max && proj_p1_max < proj_p2_min) || (proj_p1_min > proj_p2_max && proj_p1_max > proj_p2_min)) {
				if (p1_response == 0.0f && p2_response == 0.0f) {
					return false;
				}
				collision = false;
			}

			if (std::min(proj_p1_max, proj_p2_max) - std::max(proj_p1_min, proj_p2_min) < overlap) {
				overlap = std::min(proj_p1_max, proj_p2_max) - std::max(proj_p1_min, proj_p2_min);
				first = proj_p1_max < proj_p2_max;
				axis = proj_axis;
			}
		}

		if (collision) {
			axis = axis / sqrt(axis.x * axis.x + axis.y * axis.y);

			if (first) axis *= -1.0f;

			if (p1_movement) {
				*p1_movement = axis * overlap * p1_response;
			}

			if (p2_movement) {
				*p2_movement = axis * overlap * -p2_response;
			}
		}

		return collision;
	}

	bool CircleCollide(vec2* p1, size_t p1_size, float p1_response, vec2* p1_movement, vec2* p1_rect_pos, vec2* p1_rect_size, vec2 p2, float r2, float p2_response, vec2* p2_movement, vec2* p2_rect_pos, vec2* p2_rect_size) {
		bool collision = true;

		float overlap = INFINITY;
		bool first = false;
		vec2 axis;

		if (p2_rect_pos && p2_rect_size) {
			r2 = ConvertPosition(vec2(p2.x + r2, 0.0f), p2_rect_pos, p2_rect_size).x;
			p2 = ConvertPosition(p2, p2_rect_pos, p2_rect_size);
			r2 -= p2.x;
		}

		for (size_t i = 0; i < p1_size+1; i++) {
			vec2 point1, point2;

			if (i < p1_size) {
				point1 = ConvertPosition(p1[i], p1_rect_pos, p1_rect_size);
				point2 = ConvertPosition(p1[(i + 1) % p1_size], p1_rect_pos, p1_rect_size);
			}
			else {
				point2 = p2;

				vec2 min;
				float minDistance = INFINITY;

				for (size_t i = 0; i < p1_size; i++) {
					vec2 buf = p2 - ConvertPosition(p1[i], p1_rect_pos, p1_rect_size);

					float distance = buf.x * buf.x + buf.y * buf.y;

					if (distance < minDistance) {
						minDistance = distance;
						min = ConvertPosition(p1[i], p1_rect_pos, p1_rect_size);
					}
				}

				point1 = min;
			}

			vec2 proj_axis = vec2(-point2.y + point1.y, point2.x - point1.x);

			proj_axis = proj_axis / proj_axis.length();;

			if (i >= p1_size) {
				proj_axis = vec2(-proj_axis.y, proj_axis.x);
			}

			float proj_p1_min = INFINITY, proj_p1_max = -INFINITY, proj_p2_min = INFINITY, proj_p2_max = -INFINITY;

			for (size_t j = 0; j < p1_size; j++) {
				vec2 p1c = ConvertPosition(p1[j], p1_rect_pos, p1_rect_size);
				float proj = p1c.x * proj_axis.x + p1c.y * proj_axis.y;

				proj_p1_min = std::min(proj_p1_min, proj);
				proj_p1_max = std::max(proj_p1_max, proj);
			}
			{
				float proj = p2.x * proj_axis.x + p2.y * proj_axis.y;
				proj_p2_min = std::min(proj_p2_min, proj - r2);
				proj_p2_max = std::max(proj_p2_max, proj + r2);
			}

			if ((proj_p1_min < proj_p2_max && proj_p1_max < proj_p2_min) || (proj_p1_min > proj_p2_max && proj_p1_max > proj_p2_min)) {
				if (p1_response == 0.0f && p2_response == 0.0f) {
					return false;
				}
				collision = false;
			}

			float dist = proj_p2_max - proj_p1_min;

			if (std::abs(dist) < overlap) {
				overlap = dist;
				axis = proj_axis;
			}
		}

		if (collision) {
			axis = axis / sqrt(axis.x * axis.x + axis.y * axis.y);

			if (p1_movement) {
				*p1_movement = axis * overlap * p1_response;
			}

			if (p2_movement) {
				*p2_movement = axis * overlap * -p2_response;
			}
		}

		return collision;
	}
}

bool CircCollide(vec2 p1, float r1, float p1_response, vec2* p1_movement, vec2* p1_rect_pos, vec2* p1_rect_size, vec2 p2, float r2, float p2_response, vec2* p2_movement, vec2* p2_rect_pos, vec2* p2_rect_size) {
	if (p1_rect_pos && p1_rect_size) {
		r1 = SAT::ConvertPosition(vec2(p1.x + r1, 0.0f), p1_rect_pos, p1_rect_size).x;
		p1 = SAT::ConvertPosition(p1, p1_rect_pos, p1_rect_size);
		r1 -= p1.x;
	}

	if (p2_rect_pos && p2_rect_size) {
		r2 = SAT::ConvertPosition(vec2(p2.x + r2, 0.0f), p2_rect_pos, p2_rect_size).x;
		p2 = SAT::ConvertPosition(p2, p2_rect_pos, p2_rect_size);
		r2 -= p2.x;
	}
	
	vec2 dist = p2 - p1;
	float r = r1 + r2;

	bool collision = dist.x * dist.x + dist.y * dist.y <= r * r;

	if (p1_response == 0.0f && p2_response == 0.0f) return collision;

	if (collision) {
		float dist_len = sqrt(dist.x * dist.x + dist.y * dist.y);
		float overlap = r - dist_len;
		vec2 direction = dist / dist_len;

		if (p1_movement) {
			*p1_movement = direction * overlap * -p1_response;
		}
		if (p2_movement) {
			*p2_movement = direction * overlap * p2_response;
		}
	}

	return collision;
}